#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <vector>
#include <cmath>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <memory>

#include "IsometricMath.hpp"
#include "LevelManager.hpp"
#include "Entity.hpp"
#include "EntityManager.hpp"
#include "IsometricTilemap.hpp"
#include "TextureManager.hpp"
#include "Animator.hpp"
#include "Player.hpp"
#include "GhostEntity.hpp"
#include "Props.hpp"
#include "LightSource.hpp"

enum GameState { START, PLAY, DEATH, VICTORY };

class Game {
private:


    void drawRaycastLight(sf::RenderTarget& target, sf::Vector2f centerScreen, sf::Vector2f centerGrid, float radiusPixels, sf::Color color, float coneAngleDegrees = 360.f, float facingAngleGridRad = 0.f, float intensity = 1.0f) {
        sf::Sprite sprite;
        if (coneAngleDegrees < 360.f) {
            sprite.setTexture(m_flashlightTex);
            sprite.setRotation(facingAngleGridRad * 180.f / 3.14159265f);
        } else {
            sprite.setTexture(m_lightTex);
        }
        sf::FloatRect bounds = sprite.getLocalBounds();
        if (bounds.width > 0.f && bounds.height > 0.f) {
            sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
            sprite.setScale((radiusPixels * 2.f) / bounds.width, (radiusPixels * 2.f) / bounds.height);
        }
        sprite.setPosition(centerScreen);
        
        float finalAlpha = std::min(intensity * 255.f, 255.f);
        sprite.setColor(sf::Color(color.r, color.g, color.b, static_cast<sf::Uint8>(finalAlpha)));
        target.draw(sprite, sf::BlendAdd);
    }

    void renderFPV() {
        float playerAngle = std::atan2(m_player->velocity.y, m_player->velocity.x);
        static float storedLastAngle = 0.f;
        if (std::abs(m_player->velocity.x) < 0.01f && std::abs(m_player->velocity.y) < 0.01f) {
            playerAngle = storedLastAngle;
        } else {
            sf::Vector2f dirGrid = IsometricMath::isometricToCartesian(sf::Vector2f(m_player->velocity.x * 2.f, m_player->velocity.y));
            playerAngle = std::atan2(dirGrid.y, dirGrid.x);
            storedLastAngle = playerAngle;
        }

        sf::Vector2f pos = m_player->gridPosition;
        sf::Vector2f dir(std::cos(playerAngle), std::sin(playerAngle));
        float fov = 0.66f;
        sf::Vector2f plane(-dir.y * fov, dir.x * fov);

        for (int x = 0; x < FPV_WIDTH; x++) {
            float cameraX = 2 * x / (float)FPV_WIDTH - 1;
            sf::Vector2f rayDir = dir + plane * cameraX;
            
            int mapX = static_cast<int>(std::floor(pos.x));
            int mapY = static_cast<int>(std::floor(pos.y));
            
            sf::Vector2f sideDist;
            sf::Vector2f deltaDist(std::abs(1.0f / (rayDir.x + 0.000001f)), std::abs(1.0f / (rayDir.y + 0.000001f)));
            float perpWallDist;
            
            int stepX, stepY;
            int hit = 0;
            int side;
            
            if (rayDir.x < 0) {
                stepX = -1;
                sideDist.x = (pos.x - mapX) * deltaDist.x;
            } else {
                stepX = 1;
                sideDist.x = (mapX + 1.0f - pos.x) * deltaDist.x;
            }
            if (rayDir.y < 0) {
                stepY = -1;
                sideDist.y = (pos.y - mapY) * deltaDist.y;
            } else {
                stepY = 1;
                sideDist.y = (mapY + 1.0f - pos.y) * deltaDist.y;
            }
            
            int maxDepth = 24;
            while (hit == 0 && maxDepth > 0) {
                if (sideDist.x < sideDist.y) {
                    sideDist.x += deltaDist.x;
                    mapX += stepX;
                    side = 0;
                } else {
                    sideDist.y += deltaDist.y;
                    mapY += stepY;
                    side = 1;
                }
                
                int wrappedX = IsometricMath::wrapIndex(mapX, m_gridW);
                int wrappedY = IsometricMath::wrapIndex(mapY, m_gridH);
                int cellType = m_dungeonGrid[wrappedX][wrappedY];
                if (cellType != 0 && cellType != 6 && cellType != 7) {
                    hit = cellType;
                }
                maxDepth--;
            }
            
            if (hit > 0) {
                if (side == 0) perpWallDist = (mapX - pos.x + (1 - stepX) / 2) / (rayDir.x + 0.000001f);
                else           perpWallDist = (mapY - pos.y + (1 - stepY) / 2) / (rayDir.y + 0.000001f);
                
                int lineHeight = (int)(FPV_HEIGHT / (perpWallDist + 0.0001f));
                int drawStart = -lineHeight / 2 + FPV_HEIGHT / 2;
                if (drawStart < 0) drawStart = 0;
                int drawEnd = lineHeight / 2 + FPV_HEIGHT / 2;
                if (drawEnd >= FPV_HEIGHT) drawEnd = FPV_HEIGHT - 1;
                
                sf::Color color;
                if (hit == 1 || hit == 4) color = sf::Color(100, 100, 100);
                else if (hit == 3) color = sf::Color(80, 80, 120);
                else if (hit == 8) color = sf::Color(139, 69, 19);
                else if (hit == 9) color = sf::Color(101, 67, 33);
                else color = sf::Color(120, 120, 120);
                
                if (side == 1) {
                    color.r /= 2; color.g /= 2; color.b /= 2;
                }
                
                // Distance fog
                float fogDist = perpWallDist / 12.0f;
                if (fogDist > 1.0f) fogDist = 1.0f;
                sf::Color fogColor(15, 15, 20);
                color.r = static_cast<sf::Uint8>(color.r * (1.0f - fogDist) + fogColor.r * fogDist);
                color.g = static_cast<sf::Uint8>(color.g * (1.0f - fogDist) + fogColor.g * fogDist);
                color.b = static_cast<sf::Uint8>(color.b * (1.0f - fogDist) + fogColor.b * fogDist);

                for (int y = 0; y < drawStart; y++) m_fpvImage.setPixel(x, y, sf::Color(30, 30, 40));
                for (int y = drawStart; y < drawEnd; y++) m_fpvImage.setPixel(x, y, color);
                for (int y = drawEnd; y < FPV_HEIGHT; y++) m_fpvImage.setPixel(x, y, sf::Color(40, 40, 40));
            } else {
                for (int y = 0; y < FPV_HEIGHT; y++) m_fpvImage.setPixel(x, y, sf::Color(15, 15, 20));
            }
        }
        
        m_fpvTexture.update(m_fpvImage);
    }

    sf::RenderWindow m_window;
    sf::Shader m_postShader;
    bool m_shaderLoaded;
    sf::Font m_font;
    sf::RenderTexture m_sceneTexture;
    sf::RenderTexture m_lightMap;
    const int FPV_WIDTH = 320;
    const int FPV_HEIGHT = 180;
    sf::Image m_fpvImage;
    sf::Texture m_fpvTexture;
    sf::Sprite m_fpvSprite;
    sf::Texture m_lightTex;
    sf::Texture m_flashlightTex;


    // Game state
    GameState m_state;
    float m_totalTime;
    float m_heartbeatTimer;
    int m_heartbeatPulse;
    sf::View m_gameView;
    bool m_fKeyPressed;

    // Game data & level
    LevelData m_level;
    std::vector<std::vector<int>> m_dungeonGrid;
    int m_gridW;
    int m_gridH;

    // Renderers & Managers
    IsometricTilemap m_tilemap;
    EntityManager m_entityManager;
    TextureManager m_texManager;

    // Key Entities
    Player* m_player;
    GhostEntity* m_stalker;
    WeepingGhostEntity* m_weeper;
    InteractiveEntity* m_genA;
    InteractiveEntity* m_genB;
    InteractiveEntity* m_genC;
    InteractiveEntity* m_portal;

    // Interactive Lights
    std::unique_ptr<LightSource> m_playerLight;
    std::unique_ptr<LightSource> m_genALight;
    std::unique_ptr<LightSource> m_genBLight;
    std::unique_ptr<LightSource> m_genCLight;
    std::unique_ptr<LightSource> m_portalLight;
    std::vector<LightSource> m_dungeonTorches;
    std::vector<sf::Vector2f> m_baseTorchPositions;

    // Audio Resources
    sf::SoundBuffer m_soundBuffer;
    sf::Sound m_heartbeatSound;
    sf::Sound m_jumpscareSound;
    sf::Sound m_generatorSound;
    sf::Music m_bgMusic;

    // HUD Elements
    sf::Text m_promptText;
    sf::Text m_hudText;
    sf::RectangleShape m_staminaBG;
    sf::RectangleShape m_staminaBar;
    sf::RectangleShape m_fearBG;
    sf::RectangleShape m_fearBar;

    // Mission State Variables
    bool m_readJournal;
    bool m_hasJournalKey;
    std::string m_activeNoteText;

    void initWindow() {
        m_window.create(sf::VideoMode(1280, 720), "Isometric Horror Dungeon: The Runes of escape", sf::Style::Titlebar | sf::Style::Close);
        m_window.setFramerateLimit(60);
    }

    void initResources() {
        // Load Post-processing shaders (vertex and fragment)
        m_shaderLoaded = m_postShader.loadFromFile("assets/horror.vert", "assets/horror.frag");
        if (m_shaderLoaded) {
            m_postShader.setUniform("texture", sf::Shader::CurrentTexture);
        } else {
            std::cerr << "Warning: Shaders (horror.vert/horror.frag) failed to load, falling back to software mode." << std::endl;
        }

        // Load Font
        if (!m_font.loadFromFile("assets/Arial.ttf")) {
            std::cerr << "Error: Font file not found!" << std::endl;
        }

        // Load Sound Effect
        if (m_soundBuffer.loadFromFile("assets/laser.wav")) {
            m_heartbeatSound.setBuffer(m_soundBuffer);
            m_jumpscareSound.setBuffer(m_soundBuffer);
            m_generatorSound.setBuffer(m_soundBuffer);
        } else {
            std::cerr << "Warning: Could not load assets/laser.wav!" << std::endl;
        }

        // Load drone loop
        if (m_bgMusic.openFromFile("assets/music.ogg")) {
            m_bgMusic.setLoop(true);
            m_bgMusic.setVolume(20.f);
            m_bgMusic.play();
        }

        m_sceneTexture.create(1280, 720);
        m_lightTex = createLightTexture(256);
        m_flashlightTex = createFlashlightConeTexture(256, 65.f); // 65-degree flashlight cone
        m_lightMap.create(1280, 720);
        m_fpvImage.create(FPV_WIDTH, FPV_HEIGHT, sf::Color::Black);
        m_fpvTexture.loadFromImage(m_fpvImage);
        m_fpvSprite.setTexture(m_fpvTexture);
        m_fpvSprite.setPosition(1280.f - FPV_WIDTH * 1.2f - 20.f, 20.f);
        m_fpvSprite.setScale(1.2f, 1.2f);
    }

    void initLevel() {
        m_level = LevelManager::loadLevel(1);
        m_dungeonGrid = m_level.grid;
        m_gridW = m_dungeonGrid.size();
        m_gridH = m_dungeonGrid[0].size();

        m_tilemap.load(m_dungeonGrid, createTextureAtlas());

        m_entityManager.clear();

        // Create player
        m_player = new Player(m_texManager);
        m_player->gridPosition = m_level.playerSpawn;
        m_entityManager.addEntity(m_player);

        // Create walls
        sf::Texture& wallTexRight = m_texManager.getTexture("wall_right", "assets/kenney_isometricDungeon/Isometric/stoneWall_S.png");
        sf::Texture& wallTexLeft = m_texManager.getTexture("wall_left", "assets/kenney_isometricDungeon/Isometric/stoneWall_E.png");
        for (int x = 0; x < m_gridW; ++x) {
            for (int y = 0; y < m_gridH; ++y) {
                if (m_dungeonGrid[x][y] == 1) {
                    m_entityManager.addEntity(new WallEntity(wallTexRight, sf::Vector2f(static_cast<float>(x), static_cast<float>(y))));
                } else if (m_dungeonGrid[x][y] == 4) {
                    m_entityManager.addEntity(new WallEntity(wallTexLeft, sf::Vector2f(static_cast<float>(x), static_cast<float>(y))));
                }
            }
        }

        // Spawn Pillars
        sf::Texture& pillarTexture = m_texManager.getTexture("pillar", "assets/kenney_isometricDungeon/Isometric/stoneColumn_E.png");
        for (const auto& pillarPos : m_level.pillarSpawns) {
            m_entityManager.addEntity(new PillarEntity(pillarTexture, pillarPos));
        }

        // Spawn detail props
        sf::Texture& barrelTex = m_texManager.getTexture("barrel", "assets/kenney_isometricDungeon/Isometric/barrel_E.png");
        sf::Texture& crateTex = m_texManager.getTexture("crate", "assets/kenney_isometricDungeon/Isometric/woodenCrate_E.png");
        sf::Texture& chestTex = m_texManager.getTexture("chest", "assets/kenney_isometricDungeon/Isometric/chestClosed_E.png");
        sf::Texture& chairTex = m_texManager.getTexture("chair", "assets/kenney_isometricDungeon/Isometric/chair_E.png");
        std::vector<sf::Texture*> propTextures = { &barrelTex, &crateTex, &chestTex, &chairTex };

        int propsSpawned = 0;
        while (propsSpawned < 15) {
            int rx = std::rand() % m_gridW;
            int ry = std::rand() % m_gridH;
            if (m_dungeonGrid[rx][ry] == 0 && 
                std::abs(rx - m_level.playerSpawn.x) > 2.0f && 
                std::abs(ry - m_level.playerSpawn.y) > 2.0f &&
                std::abs(rx - m_level.portalSpawn.x) > 2.0f &&
                std::abs(ry - m_level.portalSpawn.y) > 2.0f &&
                // Do not spawn detail props right around the farmhouse to prevent blocking doors/gaps
                !(rx >= 2 && rx <= 12 && ry >= 20 && ry <= 29)) {
                
                int texIdx = std::rand() % propTextures.size();
                PropEntity* prop = new PropEntity(*propTextures[texIdx], sf::Vector2f(rx + 0.5f, ry + 0.5f));
                m_entityManager.addEntity(prop);
                
                m_dungeonGrid[rx][ry] = 5; // Occupied
                propsSpawned++;
            }
        }

        // Create Altars
        sf::Texture& generatorTex = m_texManager.getTexture("generator", "assets/kenney_isometricDungeon/Isometric/stoneColumnWood_E.png");
        m_genA = new InteractiveEntity(generatorTex, m_level.genASpawn + sf::Vector2f(0.5f, 0.5f), sf::Color(255, 120, 120));
        m_genB = new InteractiveEntity(generatorTex, m_level.genBSpawn + sf::Vector2f(0.5f, 0.5f), sf::Color(255, 120, 120));
        m_genC = new InteractiveEntity(generatorTex, m_level.genCSpawn + sf::Vector2f(0.5f, 0.5f), sf::Color(255, 120, 120));
        m_entityManager.addEntity(m_genA);
        m_entityManager.addEntity(m_genB);
        m_entityManager.addEntity(m_genC);

        // Escape Portal
        sf::Texture& portalTex = m_texManager.getTexture("portal", "assets/kenney_isometricDungeon/Isometric/stoneWallArchway_E.png");
        m_portal = new InteractiveEntity(portalTex, m_level.portalSpawn + sf::Vector2f(0.5f, 0.5f), sf::Color(140, 50, 220, 100));
        m_entityManager.addEntity(m_portal);

        m_dungeonGrid[static_cast<int>(m_level.genASpawn.x)][static_cast<int>(m_level.genASpawn.y)] = 5;
        m_dungeonGrid[static_cast<int>(m_level.genBSpawn.x)][static_cast<int>(m_level.genBSpawn.y)] = 5;
        m_dungeonGrid[static_cast<int>(m_level.genCSpawn.x)][static_cast<int>(m_level.genCSpawn.y)] = 5;
        m_dungeonGrid[static_cast<int>(m_level.portalSpawn.x)][static_cast<int>(m_level.portalSpawn.y)] = 5;

        // Load Farm and House textures
        sf::Texture& woodWallE = m_texManager.getTexture("wood_wall_e", "assets/kenney_isometricMiniatureFarm/Isometric/woodWall_E.png");
        sf::Texture& woodWallS = m_texManager.getTexture("wood_wall_s", "assets/kenney_isometricMiniatureFarm/Isometric/woodWall_S.png");
        sf::Texture& woodDoorE = m_texManager.getTexture("wood_door_e", "assets/kenney_isometricMiniatureFarm/Isometric/woodWallDoorOpen_E.png");
        sf::Texture& woodWindowE = m_texManager.getTexture("wood_window_e", "assets/kenney_isometricMiniatureFarm/Isometric/woodWallWindow_E.png");
        
        sf::Texture& fenceE = m_texManager.getTexture("fence_e", "assets/kenney_isometricMiniatureFarm/Isometric/fenceLow_E.png");
        sf::Texture& fenceS = m_texManager.getTexture("fence_s", "assets/kenney_isometricMiniatureFarm/Isometric/fenceLow_S.png");
        sf::Texture& cropE = m_texManager.getTexture("crop_e", "assets/kenney_isometricMiniatureFarm/Isometric/corn_E.png");
        sf::Texture& hayE = m_texManager.getTexture("hay_e", "assets/kenney_isometricMiniatureFarm/Isometric/hayBales_E.png");
        
        sf::Texture& chimneyBaseE = m_texManager.getTexture("chimney_base_e", "assets/kenney_isometricMiniatureFarm/Isometric/chimneyBase_E.png");
        sf::Texture& chimneyTopE = m_texManager.getTexture("chimney_top_e", "assets/kenney_isometricMiniatureFarm/Isometric/chimneyTop_E.png");
        sf::Texture& sackE = m_texManager.getTexture("sack_e", "assets/kenney_isometricMiniatureFarm/Isometric/sack_E.png");
        sf::Texture& sacksCrateE = m_texManager.getTexture("sacks_crate_e", "assets/kenney_isometricMiniatureFarm/Isometric/sacksCrate_E.png");

        // Spawn Wood Walls
        for (const auto& wallPos : m_level.houseWallSpawns) {
            if (static_cast<int>(wallPos.y) == 22 || static_cast<int>(wallPos.y) == 28) {
                m_entityManager.addEntity(new PropEntity(woodWallS, wallPos));
            } else {
                m_entityManager.addEntity(new PropEntity(woodWallE, wallPos));
            }
        }

        // Spawn Doors
        for (const auto& doorPos : m_level.houseWallDoorSpawns) {
            m_entityManager.addEntity(new PropEntity(woodDoorE, doorPos));
        }

        // Spawn Windows
        for (const auto& windowPos : m_level.houseWallWindowSpawns) {
            m_entityManager.addEntity(new PropEntity(woodWindowE, windowPos));
        }

        // Spawn Fences
        for (const auto& fencePos : m_level.farmFenceSpawns) {
            if (static_cast<int>(fencePos.x) == 24 || static_cast<int>(fencePos.x) == 29) {
                m_entityManager.addEntity(new PropEntity(fenceE, fencePos));
            } else {
                m_entityManager.addEntity(new PropEntity(fenceS, fencePos));
            }
        }

        // Spawn Crops
        for (const auto& cropPos : m_level.farmCropSpawns) {
            if (std::rand() % 2 == 0) {
                m_entityManager.addEntity(new PropEntity(cropE, cropPos));
            } else {
                sf::Texture& cropDouble = m_texManager.getTexture("crop_double", "assets/kenney_isometricMiniatureFarm/Isometric/cornDouble_E.png");
                m_entityManager.addEntity(new PropEntity(cropDouble, cropPos));
            }
        }

        // Spawn Hay Bales
        for (const auto& hayPos : m_level.farmHaySpawns) {
            if (std::rand() % 2 == 0) {
                m_entityManager.addEntity(new PropEntity(hayE, hayPos));
            } else {
                sf::Texture& hayStacked = m_texManager.getTexture("hay_stacked", "assets/kenney_isometricMiniatureFarm/Isometric/hayBalesStacked_E.png");
                m_entityManager.addEntity(new PropEntity(hayStacked, hayPos));
            }
        }

        // Create chimney inside the house
        sf::Vector2f chimneyPos(4.5f, 23.5f);
        m_entityManager.addEntity(new PropEntity(chimneyBaseE, chimneyPos));
        PropEntity* chimneyTop = new PropEntity(chimneyTopE, chimneyPos);
        chimneyTop->screenPosition.y -= 40.f; // stack height
        chimneyTop->sprite.setPosition(chimneyTop->screenPosition);
        m_entityManager.addEntity(chimneyTop);

        // Create sacks and crates inside
        m_entityManager.addEntity(new PropEntity(sackE, sf::Vector2f(10.5f, 23.5f)));
        m_entityManager.addEntity(new PropEntity(sacksCrateE, sf::Vector2f(10.5f, 24.5f)));
        
        // Add a table/chair set inside the house
        sf::Texture& tableTex = m_texManager.getTexture("table", "assets/kenney_isometricDungeon/Isometric/tableShort_E.png");
        m_entityManager.addEntity(new PropEntity(tableTex, sf::Vector2f(7.5f, 24.5f)));
        m_entityManager.addEntity(new PropEntity(chairTex, sf::Vector2f(7.5f, 23.5f)));
        m_entityManager.addEntity(new PropEntity(chairTex, sf::Vector2f(7.5f, 25.5f)));

        // Spawn Wall Holes (walkable entryways)
        sf::Texture& woodWallHoleE = m_texManager.getTexture("wood_wall_hole_e", "assets/kenney_isometricMiniatureFarm/Isometric/woodWallHole_E.png");
        for (const auto& holePos : m_level.houseWallHoleSpawns) {
            m_entityManager.addEntity(new PropEntity(woodWallHoleE, holePos));
        }

        // Closet wardrobe hiding spot
        sf::Texture& wardrobeTex = m_texManager.getTexture("wardrobe", "assets/kenney_isometricDungeon/Isometric/woodenCrates_E.png");
        m_entityManager.addEntity(new PropEntity(wardrobeTex, sf::Vector2f(10.5f, 26.5f)));

        // Ghosts
        m_stalker = new GhostEntity(m_texManager);
        m_stalker->spawn(m_player->gridPosition, m_gridW, m_gridH);
        m_stalker->spawnCooldown = 5.f; // 5s startup safety buffer
        m_entityManager.addEntity(m_stalker);

        m_weeper = new WeepingGhostEntity(m_texManager);
        m_weeper->spawn(m_player->gridPosition, m_gridW, m_gridH);
        m_weeper->spawnCooldown = 8.f; // 8s startup safety buffer
        m_entityManager.addEntity(m_weeper);

        // Initialize Lights
        m_playerLight = std::make_unique<LightSource>(m_player->screenPosition, 280.f, m_flashlightTex, m_player->gridPosition);
        m_playerLight->sprite.setRotation(90.f); // Facing South initially
        m_genALight = std::make_unique<LightSource>(m_genA->screenPosition - sf::Vector2f(0.f, 20.f), 130.f, m_lightTex, m_genA->gridPosition);
        m_genBLight = std::make_unique<LightSource>(m_genB->screenPosition - sf::Vector2f(0.f, 20.f), 130.f, m_lightTex, m_genB->gridPosition);
        m_genCLight = std::make_unique<LightSource>(m_genC->screenPosition - sf::Vector2f(0.f, 20.f), 130.f, m_lightTex, m_genC->gridPosition);
        m_portalLight = std::make_unique<LightSource>(m_portal->screenPosition - sf::Vector2f(0.f, 30.f), 180.f, m_lightTex, m_portal->gridPosition);
        m_portalLight->sprite.setColor(sf::Color(160, 50, 240, 200));

        m_dungeonTorches.clear();
        m_baseTorchPositions.clear();
        int torchesAdded = 0;
        const auto& entities = m_entityManager.getEntities();
        for (const auto* ent : entities) {
            if (dynamic_cast<const PillarEntity*>(ent) && torchesAdded < 4) {
                sf::Vector2f torchPos = ent->screenPosition - sf::Vector2f(0.f, 40.f);
                m_dungeonTorches.push_back(LightSource(torchPos, 140.f, m_lightTex, ent->gridPosition));
                m_baseTorchPositions.push_back(torchPos);
                torchesAdded++;
            }
        }



        m_gameView.setSize(1280.f, 720.f);
        m_gameView.setCenter(m_player->screenPosition - sf::Vector2f(0.f, 40.f));
    }

    void initHUD() {
        m_promptText.setFont(m_font);
        m_promptText.setCharacterSize(22);
        m_promptText.setFillColor(sf::Color(255, 230, 100));

        m_hudText.setFont(m_font);
        m_hudText.setCharacterSize(16);
        m_hudText.setFillColor(sf::Color(226, 232, 240));

        m_staminaBG.setSize(sf::Vector2f(200.f, 10.f));
        m_staminaBG.setFillColor(sf::Color(40, 40, 40, 180));
        m_staminaBG.setPosition(105.f, 82.f);

        m_staminaBar.setSize(sf::Vector2f(200.f, 10.f));
        m_staminaBar.setFillColor(sf::Color(46, 204, 113));
        m_staminaBar.setPosition(105.f, 82.f);

        m_fearBG.setSize(sf::Vector2f(200.f, 10.f));
        m_fearBG.setFillColor(sf::Color(40, 40, 40, 180));
        m_fearBG.setPosition(105.f, 102.f);

        m_fearBar.setSize(sf::Vector2f(200.f, 10.f));
        m_fearBar.setFillColor(sf::Color(231, 76, 60));
        m_fearBar.setPosition(105.f, 102.f);
    }

    void processEvents() {
        sf::Event event;
        while (m_window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                m_window.close();
            }
            if (event.type == sf::Event::KeyPressed) {
                // Dismiss story journal popup when active
                if (m_activeNoteText != "") {
                    if (event.key.code == sf::Keyboard::Enter || event.key.code == sf::Keyboard::E) {
                        m_activeNoteText = "";
                    }
                    continue;
                }

                if (event.key.code == sf::Keyboard::Escape) {
                    m_window.close();
                }

                if (event.key.code == sf::Keyboard::R && (m_state == DEATH || m_state == VICTORY)) {
                      resetGame();
                }

                if (event.key.code == sf::Keyboard::Enter && m_state == START) {
                    m_state = PLAY;
                }
            }
        }
    }

    void resetGame() {
        m_player->gridPosition = m_level.playerSpawn;
        m_player->fear = 0.f;
        m_player->stamina = 100.f;
        m_player->lanternOn = true;
        m_player->isHiding = false;
        m_player->sprite.setColor(sf::Color(255, 255, 255, 255));
        
        m_genA->setActive(false);
        m_genB->setActive(false);
        m_genC->setActive(false);
        m_portal->setActive(false);
        m_dungeonGrid[static_cast<int>(m_level.portalSpawn.x)][static_cast<int>(m_level.portalSpawn.y)] = 5;

        m_readJournal = false;
        m_hasJournalKey = false;
        m_activeNoteText = "";

        m_stalker->spawn(m_player->gridPosition, m_gridW, m_gridH);
        m_stalker->spawnCooldown = 5.f; // 5s safety buffer on reset
        m_weeper->spawn(m_player->gridPosition, m_gridW, m_gridH);
        m_weeper->spawnCooldown = 8.f; // 8s safety buffer on reset
        m_playerLight->sprite.setRotation(90.f); // Reset rotation to South



        m_state = PLAY;
    }

    void update(float dt) {
        if (m_state != PLAY) return;

        // Pause gameplay updates when story note is active
        if (m_activeNoteText != "") return;

        m_totalTime += dt;

        // Flashlight key toggle [F]
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::F)) {
            if (!m_fKeyPressed) {
                m_player->lanternOn = !m_player->lanternOn;
                m_fKeyPressed = true;
            }
        } else {
            m_fKeyPressed = false;
        }

        m_player->updateWithCollision(dt, m_dungeonGrid);

        // Wrap player coordinate
        float wrappedX = IsometricMath::wrapCoord(m_player->gridPosition.x, static_cast<float>(m_gridW));
        float wrappedY = IsometricMath::wrapCoord(m_player->gridPosition.y, static_cast<float>(m_gridH));
        float dx_wrap = wrappedX - m_player->gridPosition.x;
        float dy_wrap = wrappedY - m_player->gridPosition.y;

        if (dx_wrap != 0.f || dy_wrap != 0.f) {
            m_player->gridPosition = sf::Vector2f(wrappedX, wrappedY);
            sf::Vector2f tileCenter = IsometricMath::cartesianToIsometric(m_player->gridPosition);
            m_player->screenPosition = tileCenter + sf::Vector2f(0.f, 112.f);
            m_player->sprite.setPosition(m_player->screenPosition);
            
            sf::Vector2f screenOffset = IsometricMath::cartesianToIsometric(sf::Vector2f(dx_wrap, dy_wrap));
            m_gameView.move(screenOffset);
        }

        // Update Ghost logic - always pass real player position; hiding handled via player->isHiding flag
        bool ghostDetectLantern = m_player->lanternOn && !m_player->isHiding;

        m_stalker->updateGhost(dt, m_player->gridPosition, ghostDetectLantern, m_dungeonGrid, m_gridW, m_gridH, m_player->fear, m_jumpscareSound, m_player, m_totalTime, m_level.portalSpawn);
        m_weeper->updateGhost(dt, m_player->gridPosition, m_player->lastMovementDir, ghostDetectLantern, m_dungeonGrid, m_gridW, m_gridH, m_jumpscareSound, m_player, m_totalTime, m_level.portalSpawn);

        // Fear adjustments
        float dx_stalker = IsometricMath::deltaPeriodic(m_player->gridPosition.x - m_stalker->gridPosition.x, static_cast<float>(m_gridW));
        float dy_stalker = IsometricMath::deltaPeriodic(m_player->gridPosition.y - m_stalker->gridPosition.y, static_cast<float>(m_gridH));
        float dist_stalker = std::sqrt(dx_stalker * dx_stalker + dy_stalker * dy_stalker);

        float dx_weeper = IsometricMath::deltaPeriodic(m_player->gridPosition.x - m_weeper->gridPosition.x, static_cast<float>(m_gridW));
        float dy_weeper = IsometricMath::deltaPeriodic(m_player->gridPosition.y - m_weeper->gridPosition.y, static_cast<float>(m_gridH));
        float dist_weeper = std::sqrt(dx_weeper * dx_weeper + dy_weeper * dy_weeper);

        bool stalkerNear = (dist_stalker < 3.2f) && !m_player->isHiding;
        bool weeperNear = (dist_weeper < 3.2f) && !m_weeper->isFrozen && !m_player->isHiding;
        
        if (m_player->isHiding) {
            // Calm down quickly inside hiding closet
            m_player->fear -= 15.f * dt;
        } else {
            if (!m_player->lanternOn) {
                m_player->fear += 4.5f * dt;
            }
            if (stalkerNear) {
                m_player->fear += 12.f * dt;
            }
            if (weeperNear) {
                m_player->fear += 20.f * dt;
            }
        }

        // Safe recovery close to active altars (only if not hiding)
        float dx_genA = IsometricMath::deltaPeriodic(m_player->gridPosition.x - (m_level.genASpawn.x + 0.5f), static_cast<float>(m_gridW));
        float dy_genA = IsometricMath::deltaPeriodic(m_player->gridPosition.y - (m_level.genASpawn.y + 0.5f), static_cast<float>(m_gridH));
        float dist_genA = std::sqrt(dx_genA * dx_genA + dy_genA * dy_genA);

        float dx_genB = IsometricMath::deltaPeriodic(m_player->gridPosition.x - (m_level.genBSpawn.x + 0.5f), static_cast<float>(m_gridW));
        float dy_genB = IsometricMath::deltaPeriodic(m_player->gridPosition.y - (m_level.genBSpawn.y + 0.5f), static_cast<float>(m_gridH));
        float dist_genB = std::sqrt(dx_genB * dx_genB + dy_genB * dy_genB);

        float dx_genC = IsometricMath::deltaPeriodic(m_player->gridPosition.x - (m_level.genCSpawn.x + 0.5f), static_cast<float>(m_gridW));
        float dy_genC = IsometricMath::deltaPeriodic(m_player->gridPosition.y - (m_level.genCSpawn.y + 0.5f), static_cast<float>(m_gridH));
        float dist_genC = std::sqrt(dx_genC * dx_genC + dy_genC * dy_genC);

        float dx_portal = IsometricMath::deltaPeriodic(m_player->gridPosition.x - (m_level.portalSpawn.x + 0.5f), static_cast<float>(m_gridW));
        float dy_portal = IsometricMath::deltaPeriodic(m_player->gridPosition.y - (m_level.portalSpawn.y + 0.5f), static_cast<float>(m_gridH));
        float dist_portal = std::sqrt(dx_portal * dx_portal + dy_portal * dy_portal);

        bool nearSafeZone = (m_genA->isActive && dist_genA < 1.8f) || 
                            (m_genB->isActive && dist_genB < 1.8f) || 
                            (m_genC->isActive && dist_genC < 1.8f);

        if (!m_player->isHiding) {
            if (nearSafeZone && !stalkerNear && !weeperNear && m_player->lanternOn) {
                m_player->fear -= 9.f * dt;
            } else if (!stalkerNear && !weeperNear && m_player->lanternOn) {
                m_player->fear -= 1.8f * dt;
            }
        }

        if (m_player->fear < 0.f) m_player->fear = 0.f;
        if (m_player->fear >= 100.f) {
            m_player->fear = 100.f;
            m_state = DEATH;
        }

        // Interactive puzzle
        int activeGens = 0;
        if (m_genA->isActive) activeGens++;
        if (m_genB->isActive) activeGens++;
        if (m_genC->isActive) activeGens++;

        std::string promptStr = "";
        
        // Altar activation check
        if (!m_genA->isActive && dist_genA < 1.4f) {
            promptStr = "Press [E] to Activate Altar A";
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
                m_genA->setActive(true);
                m_generatorSound.setPitch(0.9f);
                m_generatorSound.play();
            }
        }
        if (!m_genB->isActive && dist_genB < 1.4f) {
            promptStr = "Press [E] to Activate Altar B";
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
                m_genB->setActive(true);
                m_generatorSound.setPitch(1.0f);
                m_generatorSound.play();
            }
        }
        if (!m_genC->isActive && dist_genC < 1.4f) {
            if (!m_hasJournalKey) {
                promptStr = "Altar C is LOCKED. Search the farmhouse for the Rune Key.";
            } else {
                promptStr = "Press [E] to Activate Altar C";
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
                    m_genC->setActive(true);
                    m_generatorSound.setPitch(1.1f);
                    m_generatorSound.play();
                }
            }
        }

        // Farmhouse Custom Interactions (Note and Hiding Spot)
        float dx_table = IsometricMath::deltaPeriodic(m_player->gridPosition.x - 7.5f, static_cast<float>(m_gridW));
        float dy_table = IsometricMath::deltaPeriodic(m_player->gridPosition.y - 24.5f, static_cast<float>(m_gridH));
        float dist_table = std::sqrt(dx_table * dx_table + dy_table * dy_table);

        float dx_closet = IsometricMath::deltaPeriodic(m_player->gridPosition.x - 10.5f, static_cast<float>(m_gridW));
        float dy_closet = IsometricMath::deltaPeriodic(m_player->gridPosition.y - 26.5f, static_cast<float>(m_gridH));
        float dist_closet = std::sqrt(dx_closet * dx_closet + dy_closet * dy_closet);

        if (!m_player->isHiding) {
            // Read John's Journal
            if (dist_table < 1.8f) {
                promptStr = "Press [E] to Read Journal Note";
                static bool ePressed = false;
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
                    if (!ePressed) {
                        m_activeNoteText = "John's Journal Note:\n\n'The entities hunt in the dark. The red stalker tracks my heartbeat.\nThe blue seeker only freezes when the light shines directly at it.\n\nTo prevent them from activating the gate, I locked Altar C.\nI hid the Rune Key inside the wardrobe closet in the bedroom corner.'\n\n[Press ENTER to close]";
                        m_readJournal = true;
                        ePressed = true;
                    }
                } else {
                    ePressed = false;
                }
            }

            // Enter Hiding Closet / Wardrobe
            if (dist_closet < 1.8f) {
                static bool hPressed = false;
                static bool ePressed2 = false;

                if (!m_hasJournalKey) {
                    if (m_readJournal) {
                        promptStr = "Press [E] to Search Wardrobe for Key   |   [H] to Hide";
                        if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
                            if (!ePressed2) {
                                m_hasJournalKey = true;
                                m_activeNoteText = "John's Wardrobe Closet:\n\nSearching the bottom drawers...\n\nYou found the Altar C Rune Key!\nUse it to activate Altar C in the dungeon.\n\n[Press ENTER to close]";
                                ePressed2 = true;
                            }
                        } else {
                            ePressed2 = false;
                        }
                    } else {
                        promptStr = "Locked Wardrobe Closet (Need Clue)   |   Press [H] to Hide";
                    }
                } else {
                    promptStr = "Press [H] to Hide in Closet";
                }

                if (sf::Keyboard::isKeyPressed(sf::Keyboard::H)) {
                    if (!hPressed) {
                        m_player->isHiding = true;
                        m_player->gridPosition = sf::Vector2f(10.5f, 26.5f);
                        m_player->sprite.setColor(sf::Color(255, 255, 255, 60)); // Highly translucent
                        hPressed = true;
                    }
                } else {
                    hPressed = false;
                }
            }
        } else {
            // Player is Hiding
            promptStr = "HIDING. Press [H] to Exit Closet";
            static bool hPressed2 = false;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::H)) {
                if (!hPressed2) {
                    m_player->isHiding = false;
                    m_player->gridPosition = sf::Vector2f(9.5f, 26.5f); // Step outside closet
                    m_player->sprite.setColor(sf::Color(255, 255, 255, 255)); // Restore opacity
                    hPressed2 = true;
                }
            } else {
                hPressed2 = false;
            }
        }

        if (activeGens == 3) {
            m_portal->setActive(true);
            m_dungeonGrid[static_cast<int>(m_level.portalSpawn.x)][static_cast<int>(m_level.portalSpawn.y)] = 0;

            if (dist_portal < 1.4f) {
                promptStr = "Step into Portal to ESCAPE!";
                if (dist_portal < 0.7f) {
                    m_state = VICTORY;
                }
            } else {
                promptStr = "PORTAL ACTIVE! Reach the center Archway!";
            }
        }

        m_promptText.setString(promptStr);
        sf::FloatRect promptBounds = m_promptText.getLocalBounds();
        m_promptText.setOrigin(promptBounds.width / 2.f, promptBounds.height / 2.f);
        m_promptText.setPosition(640.f, 600.f);

        // Heartbeat
        m_heartbeatTimer -= dt;
        if (m_heartbeatTimer <= 0.f) {
            float fearFactor = m_player->fear / 100.f;
            float interval = 1.25f - 0.90f * fearFactor;
            m_heartbeatSound.setPitch(0.18f);
            m_heartbeatSound.setVolume(20.f + 50.f * fearFactor);
            m_heartbeatSound.play();
            m_heartbeatTimer = interval;
            m_heartbeatPulse = 1;
        } else if (m_heartbeatPulse == 1 && m_heartbeatTimer < (1.25f - 0.90f * (m_player->fear / 100.f)) - 0.16f) {
            m_heartbeatSound.setPitch(0.15f);
            m_heartbeatSound.setVolume(15.f + 40.f * (m_player->fear / 100.f));
            m_heartbeatSound.play();
            m_heartbeatPulse = 0;
        }

        // Camera follow
        sf::Vector2f targetCamCenter = m_player->screenPosition - sf::Vector2f(0.f, 40.f);
        sf::Vector2f currentCamCenter = m_gameView.getCenter();
        m_gameView.setCenter(currentCamCenter + (targetCamCenter - currentCamCenter) * 0.08f);

        // Flicker lantern
        static float sputterTimer = 0.f;
        if (sputterTimer > 0.f) {
            sputterTimer -= dt;
        } else if (std::rand() % 400 == 0) {
            sputterTimer = 0.4f + (std::rand() % 60) / 100.f;
        }

        float baseRadius = m_player->isRunning ? 200.f : 285.f;
        float flickerModifier = 1.f;
        if (sputterTimer > 0.f || m_player->fear > 70.f) {
            float scaleMod = (m_player->fear > 70.f) ? 0.45f : 1.f;
            flickerModifier = scaleMod * (0.35f + 0.3f * std::sin(m_totalTime * 35.f) * (std::rand() % 100 / 100.f));
        } else {
            flickerModifier = 1.f + 0.05f * std::sin(m_totalTime * 12.f) * std::cos(m_totalTime * 4.f);
        }

        m_playerLight->setPosition(m_player->screenPosition - sf::Vector2f(0.f, 48.f));
        m_playerLight->setRadius(baseRadius * flickerModifier);
        
        // Rotate the directional flashlight to match player's facing direction
        float dirToAngle[8] = { 315.f, 0.f, 45.f, 90.f, 135.f, 180.f, 225.f, 270.f };
        m_playerLight->sprite.setRotation(dirToAngle[m_player->currentDir]);

        m_genALight->setRadius(m_genA->isActive ? (180.f + 10.f * std::sin(m_totalTime * 15.f)) : (80.f + 15.f * std::cos(m_totalTime * 5.f)));
        m_genALight->sprite.setColor(m_genA->isActive ? sf::Color(100, 255, 100, 220) : sf::Color(255, 80, 80, 160));

        m_genBLight->setRadius(m_genB->isActive ? (180.f + 10.f * std::sin(m_totalTime * 17.f)) : (80.f + 15.f * std::cos(m_totalTime * 6.f)));
        m_genBLight->sprite.setColor(m_genB->isActive ? sf::Color(100, 255, 100, 220) : sf::Color(255, 80, 80, 160));

        m_genCLight->setRadius(m_genC->isActive ? (180.f + 10.f * std::sin(m_totalTime * 13.f)) : (80.f + 15.f * std::cos(m_totalTime * 4.f)));
        m_genCLight->sprite.setColor(m_genC->isActive ? sf::Color(100, 255, 100, 220) : sf::Color(255, 80, 80, 160));

        if (m_portal->isActive) {
            m_portalLight->setRadius(200.f + 30.f * std::sin(m_totalTime * 10.f));
        }

        for (size_t i = 0; i < m_dungeonTorches.size(); ++i) {
            m_dungeonTorches[i].setRadius(140.f + 12.f * std::sin(m_totalTime * 8.f + i * 2.f));
        }

        // HUD bars dimensions
        m_staminaBar.setSize(sf::Vector2f(m_player->stamina * 2.f, 10.f));
        m_staminaBar.setFillColor(m_player->fatigued ? sf::Color(230, 126, 34) : sf::Color(46, 204, 113));
        m_fearBar.setSize(sf::Vector2f(m_player->fear * 2.f, 10.f));

        std::stringstream ss;
        if (!m_readJournal) {
            ss << "MISSION 1: Investigate the Farmhouse for clues (Dist: " << static_cast<int>(dist_table) << "m)\n";
        } else {
            ss << "MISSION 1: [COMPLETED] Farmhouse investigated.\n";
            if (m_hasJournalKey) {
                ss << "MISSION 2: [COMPLETED] Key acquired!\n";
            } else {
                ss << "MISSION 2: Search the Wardrobe Closet for Altar C Key (Dist: " << static_cast<int>(dist_closet) << "m)\n";
            }
        }

        ss << "MISSION 3: Activate 3 Rune Altars to Open Escape Portal\n";
        if (!m_genA->isActive) {
            ss << " - Altar A: " << static_cast<int>(dist_genA) << "m\n";
        } else {
            ss << " - Altar A: [ACTIVE]\n";
        }
        if (!m_genB->isActive) {
            ss << " - Altar B: " << static_cast<int>(dist_genB) << "m\n";
        } else {
            ss << " - Altar B: [ACTIVE]\n";
        }
        if (!m_genC->isActive) {
            if (!m_hasJournalKey) {
                ss << " - Altar C: LOCKED (Rune Key Required)\n";
            } else {
                ss << " - Altar C: " << static_cast<int>(dist_genC) << "m (Key Acquired!)\n";
            }
        } else {
            ss << " - Altar C: [ACTIVE]\n";
        }

        ss << "ALTARS RUNNING: " << activeGens << " / 3\n";

        if (activeGens == 3) {
            ss << "MISSION 4: ESCAPE! Go to the Portal (Dist: " << static_cast<int>(dist_portal) << "m)\n";
        }

        ss << "LANTERN: " << (m_player->lanternOn ? "ON [F to Hide]" : "OFF [In Darkness]") << "\n"
           << "STAMINA: \nFEAR: ";

        m_hudText.setString(ss.str());
        m_hudText.setPosition(15.f, 15.f);
    }

    void render() {
        m_sceneTexture.clear(sf::Color(0, 0, 0));

        if (m_state == PLAY) {
            m_sceneTexture.setView(m_gameView);

            // Determine active repeating map sector
            int sectorX = static_cast<int>(std::floor(m_player->gridPosition.x / m_gridW));
            int sectorY = static_cast<int>(std::floor(m_player->gridPosition.y / m_gridH));

            // Draw tilemap repeatedly
            for (int dx = -1; dx <= 1; ++dx) {
                for (int dy = -1; dy <= 1; ++dy) {
                    int sx = sectorX + dx;
                    int sy = sectorY + dy;
                    m_tilemap.setPosition(IsometricMath::cartesianToIsometric(sf::Vector2f(sx * m_gridW, sy * m_gridH)));
                    m_sceneTexture.draw(m_tilemap);
                }
            }

            // Collect and depth-sort entities
            struct RenderInstance {
                Entity* entity;
                sf::Vector2f screenPos;
                float depth;
            };
            std::vector<RenderInstance> renderInstances;
            const auto& entities = m_entityManager.getEntities();
            renderInstances.reserve(entities.size() * 9);

            for (auto* ent : entities) {
                if (ent == m_player) {
                    renderInstances.push_back({m_player, m_player->screenPosition, m_player->screenPosition.y});
                } else if (ent == m_stalker) {
                    renderInstances.push_back({m_stalker, m_stalker->screenPosition, m_stalker->screenPosition.y});
                } else if (ent == m_weeper) {
                    renderInstances.push_back({m_weeper, m_weeper->screenPosition, m_weeper->screenPosition.y});
                } else {
                    for (int dx = -1; dx <= 1; ++dx) {
                        for (int dy = -1; dy <= 1; ++dy) {
                            int sx = sectorX + dx;
                            int sy = sectorY + dy;
                            sf::Vector2f offset = IsometricMath::cartesianToIsometric(sf::Vector2f(sx * m_gridW, sy * m_gridH));
                            sf::Vector2f pos = ent->screenPosition + offset;
                            renderInstances.push_back({ent, pos, pos.y});
                        }
                    }
                }
            }

            std::sort(renderInstances.begin(), renderInstances.end(), [](const RenderInstance& a, const RenderInstance& b) {
                return a.depth < b.depth;
            });

            for (const auto& inst : renderInstances) {
                inst.entity->sprite.setPosition(inst.screenPos);
                m_sceneTexture.draw(inst.entity->sprite);
            }

            // Restore screen positions
            m_player->sprite.setPosition(m_player->screenPosition);
            m_stalker->sprite.setPosition(m_stalker->screenPosition);
            m_weeper->sprite.setPosition(m_weeper->screenPosition);

            // Shroud Map lighting
            m_lightMap.setView(m_gameView);
            m_lightMap.clear(sf::Color(2, 2, 5, 253));

            if (m_player->lanternOn && !m_player->isHiding) {
                sf::Vector2f centerScreen = m_player->screenPosition - sf::Vector2f(0.f, 48.f);
                float centerAngleCart = std::atan2(m_player->lastMovementDir.y, m_player->lastMovementDir.x);
                drawRaycastLight(m_lightMap, centerScreen, m_player->gridPosition, m_playerLight->radius, sf::Color(255, 235, 190), 65.f, centerAngleCart, 1.7f);
            }

            // Draw Stalker Light
            if (m_stalker->active && m_stalker->alpha > 0.f) {
                sf::Vector2f stalkerCenterScreen = m_stalker->screenPosition - sf::Vector2f(0.f, 48.f);
                drawRaycastLight(m_lightMap, stalkerCenterScreen, m_stalker->gridPosition, 90.f, sf::Color(255, 30, 30), 360.f, 0.f, 0.82f);
            }

            // Draw repeating static lights
            for (int dx = -1; dx <= 1; ++dx) {
                for (int dy = -1; dy <= 1; ++dy) {
                    int sx = sectorX + dx;
                    int sy = sectorY + dy;
                    sf::Vector2f offset = IsometricMath::cartesianToIsometric(sf::Vector2f(sx * m_gridW, sy * m_gridH));
                    
                    // Draw generator lights
                    sf::Vector2f genAPosScreen = m_genA->screenPosition - sf::Vector2f(0.f, 20.f) + offset;
                    drawRaycastLight(m_lightMap, genAPosScreen, m_genA->gridPosition, m_genALight->radius, m_genALight->sprite.getColor(), 360.f, 0.f, 1.0f);

                    sf::Vector2f genBPosScreen = m_genB->screenPosition - sf::Vector2f(0.f, 20.f) + offset;
                    drawRaycastLight(m_lightMap, genBPosScreen, m_genB->gridPosition, m_genBLight->radius, m_genBLight->sprite.getColor(), 360.f, 0.f, 1.0f);

                    sf::Vector2f genCPosScreen = m_genC->screenPosition - sf::Vector2f(0.f, 20.f) + offset;
                    drawRaycastLight(m_lightMap, genCPosScreen, m_genC->gridPosition, m_genCLight->radius, m_genCLight->sprite.getColor(), 360.f, 0.f, 1.0f);

                    if (m_portal->isActive) {
                        sf::Vector2f portalPosScreen = m_portal->screenPosition - sf::Vector2f(0.f, 30.f) + offset;
                        drawRaycastLight(m_lightMap, portalPosScreen, m_portal->gridPosition, m_portalLight->radius, m_portalLight->sprite.getColor(), 360.f, 0.f, 1.0f);
                    }

                    for (size_t i = 0; i < m_dungeonTorches.size(); ++i) {
                        sf::Vector2f torchPosScreen = m_baseTorchPositions[i] + offset;
                        drawRaycastLight(m_lightMap, torchPosScreen, m_dungeonTorches[i].gridPosition, m_dungeonTorches[i].radius, sf::Color(255, 190, 100), 360.f, 0.f, 1.0f);
                    }
                }
            }
            m_lightMap.display();

            m_sceneTexture.setView(m_sceneTexture.getDefaultView());
            sf::Sprite lightSprite(m_lightMap.getTexture());
            m_sceneTexture.draw(lightSprite, sf::BlendMultiply);


            renderFPV();
            m_sceneTexture.draw(m_fpvSprite);

            m_sceneTexture.draw(m_hudText);
            m_sceneTexture.draw(m_staminaBG);
            m_sceneTexture.draw(m_staminaBar);
            m_sceneTexture.draw(m_fearBG);
            m_sceneTexture.draw(m_fearBar);
            if (m_promptText.getString() != "") {
                m_sceneTexture.draw(m_promptText);
            }

        } else if (m_state == START) {
            // Dark gradient background
            sf::VertexArray bgGrad(sf::Quads, 4);
            bgGrad[0].position = sf::Vector2f(0, 0); bgGrad[0].color = sf::Color(5, 3, 15);
            bgGrad[1].position = sf::Vector2f(1280, 0); bgGrad[1].color = sf::Color(5, 3, 15);
            bgGrad[2].position = sf::Vector2f(1280, 720); bgGrad[2].color = sf::Color(15, 8, 30);
            bgGrad[3].position = sf::Vector2f(0, 720); bgGrad[3].color = sf::Color(15, 8, 30);
            m_sceneTexture.draw(bgGrad);

            // Decorative line separator
            sf::RectangleShape topLine(sf::Vector2f(400.f, 1.f));
            topLine.setFillColor(sf::Color(180, 40, 50, 180));
            topLine.setOrigin(200.f, 0.f);
            topLine.setPosition(640.f, 190.f);
            m_sceneTexture.draw(topLine);

            sf::Text titleText;
            titleText.setFont(m_font);
            titleText.setCharacterSize(52);
            titleText.setFillColor(sf::Color(220, 20, 40));
            titleText.setStyle(sf::Text::Bold);
            titleText.setString("THE RUNES OF ESCAPE");
            sf::FloatRect tb = titleText.getLocalBounds();
            titleText.setOrigin(tb.width / 2.f, tb.height / 2.f);
            titleText.setPosition(640.f, 225.f);

            sf::Text subtitleText;
            subtitleText.setFont(m_font);
            subtitleText.setCharacterSize(16);
            subtitleText.setFillColor(sf::Color(140, 130, 160));
            subtitleText.setString("An Isometric Horror Survival Experience");
            sf::FloatRect stb = subtitleText.getLocalBounds();
            subtitleText.setOrigin(stb.width / 2.f, stb.height / 2.f);
            subtitleText.setPosition(640.f, 270.f);

            sf::RectangleShape botLine(sf::Vector2f(400.f, 1.f));
            botLine.setFillColor(sf::Color(180, 40, 50, 180));
            botLine.setOrigin(200.f, 0.f);
            botLine.setPosition(640.f, 295.f);
            m_sceneTexture.draw(botLine);

            sf::Text infoText;
            infoText.setFont(m_font);
            infoText.setCharacterSize(16);
            infoText.setFillColor(sf::Color(200, 205, 215));
            infoText.setString(
                "You are trapped in an infinite looping dungeon...\n\n"
                "OBJECTIVE:\n"
                "  Find and activate 3 Rune Altars, then escape through the Portal.\n\n"
                "CONTROLS:\n"
                "  WASD / Arrows : Move (screen-aligned)     Shift : Sprint\n"
                "  F : Flashlight toggle     E : Interact     H : Hide in closet\n\n"
                "ENEMIES:\n"
                "  Red STALKER - hunts your heartbeat in the light.\n"
                "    Turn off flashlight [F] to become invisible to it.\n"
                "  Blue SEEKER - moves only when unseen.\n"
                "    Shine your flashlight directly at it to freeze and repel it!"
            );
            sf::FloatRect ib = infoText.getLocalBounds();
            infoText.setOrigin(ib.width / 2.f, ib.height / 2.f);
            infoText.setPosition(640.f, 440.f);

            // Pulsing start prompt
            float startPulse = 0.6f + 0.4f * std::sin(m_totalTime * 3.f);
            sf::Text startText;
            startText.setFont(m_font);
            startText.setCharacterSize(22);
            startText.setFillColor(sf::Color(255, 230, 100, static_cast<sf::Uint8>(startPulse * 255.f)));
            startText.setString("[ PRESS ENTER TO BEGIN ]");
            sf::FloatRect sb = startText.getLocalBounds();
            startText.setOrigin(sb.width / 2.f, sb.height / 2.f);
            startText.setPosition(640.f, 640.f);

            m_sceneTexture.draw(titleText);
            m_sceneTexture.draw(subtitleText);
            m_sceneTexture.draw(infoText);
            m_sceneTexture.draw(startText);

        } else if (m_state == DEATH) {
            // Dark red gradient background
            sf::VertexArray deathBg(sf::Quads, 4);
            deathBg[0].position = sf::Vector2f(0, 0); deathBg[0].color = sf::Color(20, 2, 2);
            deathBg[1].position = sf::Vector2f(1280, 0); deathBg[1].color = sf::Color(20, 2, 2);
            deathBg[2].position = sf::Vector2f(1280, 720); deathBg[2].color = sf::Color(40, 5, 5);
            deathBg[3].position = sf::Vector2f(0, 720); deathBg[3].color = sf::Color(40, 5, 5);
            m_sceneTexture.draw(deathBg);

            sf::Text deadText;
            deadText.setFont(m_font);
            deadText.setCharacterSize(56);
            deadText.setStyle(sf::Text::Bold);
            deadText.setFillColor(sf::Color(220, 20, 20));
            deadText.setString("YOU DIED");
            sf::FloatRect tb = deadText.getLocalBounds();
            deadText.setOrigin(tb.width / 2.f, tb.height / 2.f);
            deadText.setPosition(640.f, 280.f);

            sf::Text deathSubtext;
            deathSubtext.setFont(m_font);
            deathSubtext.setCharacterSize(18);
            deathSubtext.setFillColor(sf::Color(180, 80, 80));
            deathSubtext.setString("Your fear overtook you in the deep shadows.");
            sf::FloatRect dsb = deathSubtext.getLocalBounds();
            deathSubtext.setOrigin(dsb.width / 2.f, dsb.height / 2.f);
            deathSubtext.setPosition(640.f, 350.f);

            sf::RectangleShape deathLine(sf::Vector2f(300.f, 1.f));
            deathLine.setFillColor(sf::Color(150, 30, 30, 150));
            deathLine.setOrigin(150.f, 0.f);
            deathLine.setPosition(640.f, 380.f);

            sf::Text restartText;
            restartText.setFont(m_font);
            restartText.setCharacterSize(20);
            float rPulse = 0.5f + 0.5f * std::sin(m_totalTime * 2.5f);
            restartText.setFillColor(sf::Color(200, 205, 210, static_cast<sf::Uint8>(rPulse * 255.f)));
            restartText.setString("Press [R] to Restart");
            sf::FloatRect rb = restartText.getLocalBounds();
            restartText.setOrigin(rb.width / 2.f, rb.height / 2.f);
            restartText.setPosition(640.f, 420.f);

            m_sceneTexture.draw(deadText);
            m_sceneTexture.draw(deathSubtext);
            m_sceneTexture.draw(deathLine);
            m_sceneTexture.draw(restartText);

        } else if (m_state == VICTORY) {
            // Dark green gradient background
            sf::VertexArray winBg(sf::Quads, 4);
            winBg[0].position = sf::Vector2f(0, 0); winBg[0].color = sf::Color(2, 15, 8);
            winBg[1].position = sf::Vector2f(1280, 0); winBg[1].color = sf::Color(2, 15, 8);
            winBg[2].position = sf::Vector2f(1280, 720); winBg[2].color = sf::Color(5, 30, 15);
            winBg[3].position = sf::Vector2f(0, 720); winBg[3].color = sf::Color(5, 30, 15);
            m_sceneTexture.draw(winBg);

            sf::Text winText;
            winText.setFont(m_font);
            winText.setCharacterSize(52);
            winText.setStyle(sf::Text::Bold);
            winText.setFillColor(sf::Color(46, 204, 113));
            winText.setString("YOU ESCAPED!");
            sf::FloatRect tb = winText.getLocalBounds();
            winText.setOrigin(tb.width / 2.f, tb.height / 2.f);
            winText.setPosition(640.f, 270.f);

            sf::Text winSubtext;
            winSubtext.setFont(m_font);
            winSubtext.setCharacterSize(18);
            winSubtext.setFillColor(sf::Color(100, 200, 140));
            winSubtext.setString("You activated all the Runes and stepped through the Portal to safety.");
            sf::FloatRect wsb = winSubtext.getLocalBounds();
            winSubtext.setOrigin(wsb.width / 2.f, wsb.height / 2.f);
            winSubtext.setPosition(640.f, 340.f);

            sf::RectangleShape winLine(sf::Vector2f(350.f, 1.f));
            winLine.setFillColor(sf::Color(46, 204, 113, 150));
            winLine.setOrigin(175.f, 0.f);
            winLine.setPosition(640.f, 370.f);

            sf::Text restartText;
            restartText.setFont(m_font);
            restartText.setCharacterSize(20);
            float wPulse = 0.5f + 0.5f * std::sin(m_totalTime * 2.5f);
            restartText.setFillColor(sf::Color(220, 225, 230, static_cast<sf::Uint8>(wPulse * 255.f)));
            restartText.setString("Press [R] to Play Again");
            sf::FloatRect rb = restartText.getLocalBounds();
            restartText.setOrigin(rb.width / 2.f, rb.height / 2.f);
            restartText.setPosition(640.f, 420.f);

            m_sceneTexture.draw(winText);
            m_sceneTexture.draw(winSubtext);
            m_sceneTexture.draw(winLine);
            m_sceneTexture.draw(restartText);
        }

        // Draw active Journal story note inspect overlay
        if (m_activeNoteText != "") {
            sf::RectangleShape noteBG(sf::Vector2f(600.f, 320.f));
            noteBG.setFillColor(sf::Color(10, 10, 15, 235));
            noteBG.setOutlineColor(sf::Color(180, 150, 100, 200));
            noteBG.setOutlineThickness(2.f);
            noteBG.setOrigin(300.f, 160.f);
            noteBG.setPosition(640.f, 360.f);
            m_sceneTexture.draw(noteBG);

            sf::Text noteText;
            noteText.setFont(m_font);
            noteText.setCharacterSize(18);
            noteText.setFillColor(sf::Color(230, 215, 180));
            noteText.setString(m_activeNoteText);
            sf::FloatRect ntb = noteText.getLocalBounds();
            noteText.setOrigin(ntb.width / 2.f, ntb.height / 2.f);
            noteText.setPosition(640.f, 360.f);
            m_sceneTexture.draw(noteText);
        }

        m_sceneTexture.display();

        m_window.clear();
        sf::Sprite finalSprite(m_sceneTexture.getTexture());
        
        if (m_shaderLoaded && m_state == PLAY) {
            m_postShader.setUniform("time", m_totalTime);
            m_postShader.setUniform("fear", m_player->fear / 100.f);
            m_window.draw(finalSprite, &m_postShader);
        } else {
            m_window.draw(finalSprite);
        }
        
        m_window.display();
    }

public:
    Game() 
        : m_shaderLoaded(false), m_state(START), m_totalTime(0.f), m_heartbeatTimer(0.f), m_heartbeatPulse(0), m_fKeyPressed(false),
          m_readJournal(false), m_hasJournalKey(false), m_activeNoteText("") {
        std::srand(static_cast<unsigned int>(std::time(nullptr)));
        initWindow();
        initResources();
        initLevel();
        initHUD();
    }

    void run() {
        sf::Clock clock;
        while (m_window.isOpen()) {
            float dt = clock.restart().asSeconds();
            if (dt > 0.1f) dt = 0.1f;

            processEvents();
            update(dt);
            render();
        }
    }
};
