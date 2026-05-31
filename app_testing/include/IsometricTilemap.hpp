#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include "IsometricMath.hpp"

// Generate the floor & wall texture atlas at runtime
inline sf::Texture createTextureAtlas() {
    sf::Image atlasImg;
    atlasImg.create(1024, 512, sf::Color::Transparent);

    sf::Image floorImg;
    if (floorImg.loadFromFile("assets/kenney_isometricDungeon/Isometric/stoneTile_E.png")) {
        atlasImg.copy(floorImg, 0, 0);
    } else {
        std::cerr << "Warning: Could not load floor tile texture!" << std::endl;
    }

    sf::Image wallImg;
    if (wallImg.loadFromFile("assets/kenney_isometricDungeon/Isometric/stoneWall_E.png")) {
        atlasImg.copy(wallImg, 256, 0);
    } else {
        std::cerr << "Warning: Could not load wall tile texture!" << std::endl;
    }

    sf::Image planksImg;
    if (planksImg.loadFromFile("assets/kenney_isometricMiniatureFarm/Isometric/planks_E.png")) {
        atlasImg.copy(planksImg, 512, 0);
    } else {
        std::cerr << "Warning: Could not load planks tile texture!" << std::endl;
    }

    sf::Image dirtImg;
    if (dirtImg.loadFromFile("assets/kenney_isometricMiniatureFarm/Isometric/dirtFarmland_E.png")) {
        atlasImg.copy(dirtImg, 768, 0);
    } else {
        std::cerr << "Warning: Could not load dirt farmland tile texture!" << std::endl;
    }

    sf::Texture atlasTex;
    atlasTex.loadFromImage(atlasImg);
    return atlasTex;
}

class IsometricTilemap : public sf::Drawable, public sf::Transformable {
private:
    sf::VertexArray m_vertices;
    sf::Texture m_texture;

    // Standard draw method from sf::Drawable
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
        states.transform *= getTransform();
        states.texture = &m_texture;
        target.draw(m_vertices, states);
    }

public:
    // Build the static floor vertex array using the texture atlas
    void load(const std::vector<std::vector<int>>& grid, const sf::Texture& textureAtlas) {
        m_texture = textureAtlas;
        m_vertices.setPrimitiveType(sf::Quads);
        m_vertices.clear();

        int width = grid.size();
        int height = grid[0].size();

        // Quad width and height matching scaled Kenney assets (256x512 scaled by 0.25)
        const float w = 64.f;
        const float h = 128.f;

        // Build vertex quads for each floor tile (0, 6, 7) in the grid
        for (int x = 0; x < width; ++x) {
            for (int y = 0; y < height; ++y) {
                // Floors are rendered flat as a background layer
                // We draw a floor tile under floors, walls (1), and pillars (3) (anything that isn't abyss/2)
                if (grid[x][y] != 2) {
                    sf::Vector2f screenPos = IsometricMath::cartesianToIsometric(sf::Vector2f(x, y));

                    sf::Vertex quad[4];

                    // Positional coordinates (aligned with offset so floor tiles connect seamlessly)
                    quad[0].position = sf::Vector2f(screenPos.x - w / 2.f, screenPos.y - 16.f);
                    quad[1].position = sf::Vector2f(screenPos.x + w / 2.f, screenPos.y - 16.f);
                    quad[2].position = sf::Vector2f(screenPos.x + w / 2.f, screenPos.y + 112.f);
                    quad[3].position = sf::Vector2f(screenPos.x - w / 2.f, screenPos.y + 112.f);

                    // Dynamic Texture Coordinates
                    float txStart = 0.f;
                    if (grid[x][y] == 6 || grid[x][y] == 8) {
                        txStart = 512.f; // Planks
                    } else if (grid[x][y] == 7 || grid[x][y] == 9) {
                        txStart = 768.f; // Dirt Farmland
                    }
                    float txEnd = txStart + 256.f;

                    quad[0].texCoords = sf::Vector2f(txStart, 0.f);
                    quad[1].texCoords = sf::Vector2f(txEnd, 0.f);
                    quad[2].texCoords = sf::Vector2f(txEnd, 512.f);
                    quad[3].texCoords = sf::Vector2f(txStart, 512.f);

                    for (int i = 0; i < 4; ++i) {
                        m_vertices.append(quad[i]);
                    }
                }
            }
        }
    }
};

