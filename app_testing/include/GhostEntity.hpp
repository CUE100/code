#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <cmath>
#include <string>
#include <algorithm>
#include "Entity.hpp"
#include "TextureManager.hpp"
#include "Animator.hpp"
#include "IsometricMath.hpp"
#include "Player.hpp"

// Ghost Chaser Entity (Stalker Phantasm)
class GhostEntity : public Entity {
public:
    float speed = 1.2f;
    float alpha = 0.f;
    bool active = true;
    float activeTimer = 0.f;
    float spawnCooldown = 5.f; // Delay before stalker can stalk again (safety buffer at start)
    sf::Texture* runTextures[8][10];
    sf::Texture* idleTextures[8];
    int currentDir = 0;
    Animator walkAnimator;

    GhostEntity(TextureManager& texManager) : walkAnimator(0, 0, 10, 1.f / 12.f) {
        for (int d = 0; d < 8; ++d) {
            std::string idlePath = "assets/kenney_isometricDungeon/Characters/Male/Male_" + std::to_string(d) + "_Idle0.png";
            idleTextures[d] = &texManager.getTexture("char_male_idle_" + std::to_string(d), idlePath);
            for (int f = 0; f < 10; ++f) {
                std::string runPath = "assets/kenney_isometricDungeon/Characters/Male/Male_" + std::to_string(d) + "_Run" + std::to_string(f) + ".png";
                runTextures[d][f] = &texManager.getTexture("char_male_run_" + std::to_string(d) + "_" + std::to_string(f), runPath);
            }
        }
        updateTexture(*idleTextures[0]);
        sprite.setScale(0.25f, 0.25f);
        sprite.setColor(sf::Color(220, 20, 20, 130)); // Translucent red
    }

    void updateTexture(const sf::Texture& tex) {
        sprite.setTexture(tex);
        sf::FloatRect bounds = sprite.getLocalBounds();
        sprite.setOrigin(bounds.width / 2.f, bounds.height);
    }

    void update(float dt) override {}

    void spawn(sf::Vector2f playerGridPos, int gridW, int gridH) {
        int sectorX = static_cast<int>(std::floor(playerGridPos.x / gridW));
        int sectorY = static_cast<int>(std::floor(playerGridPos.y / gridH));
        for (int i = 0; i < 30; ++i) {
            float rx = std::rand() % gridW;
            float ry = std::rand() % gridH;
            float absX = sectorX * gridW + rx + 0.5f;
            float absY = sectorY * gridH + ry + 0.5f;
            float dist = std::sqrt((absX - playerGridPos.x)*(absX - playerGridPos.x) + (absY - playerGridPos.y)*(absY - playerGridPos.y));
            if (dist > 8.f && dist < 15.f) {
                gridPosition = sf::Vector2f(absX, absY);
                alpha = 0.f;
                break;
            }
        }
    }

    void updateGhost(float dt, sf::Vector2f playerGridPos, bool playerLantern, const std::vector<std::vector<int>>& grid, int gridW, int gridH, float playerFear, sf::Sound& screamSound, Player* player, float totalTime, sf::Vector2f portalGridPos) {
        // Handle spawn cooldown (inactive, invisible, doesn't move)
        if (spawnCooldown > 0.f) {
            spawnCooldown -= dt;
            alpha = 0.f;
            sprite.setColor(sf::Color(0, 0, 0, 0));
            return;
        }

        // Track/Chase player
        float dx = IsometricMath::deltaPeriodic(playerGridPos.x - gridPosition.x, static_cast<float>(gridW));
        float dy = IsometricMath::deltaPeriodic(playerGridPos.y - gridPosition.y, static_cast<float>(gridH));
        float dist = std::sqrt(dx * dx + dy * dy);

        // Raycast check from ghost to player to verify line-of-sight
        bool hasLineOfSight = true;
        if (dist > 0.05f) {
            sf::Vector2f rayDir(dx / dist, dy / dist);
            for (float step = 0.1f; step < dist; step += 0.1f) {
                sf::Vector2f checkPos = gridPosition + rayDir * step;
                int cx = IsometricMath::wrapIndex(static_cast<int>(std::floor(checkPos.x)), gridW);
                int cy = IsometricMath::wrapIndex(static_cast<int>(std::floor(checkPos.y)), gridH);
                
                int cellType = grid[cx][cy];
                if (cellType != 0 && cellType != 6 && cellType != 7) {
                    sf::Vector2f tileCenter(static_cast<float>(cx) + 0.5f, static_cast<float>(cy) + 0.5f);
                    float dx_c = checkPos.x - tileCenter.x;
                    float dy_c = checkPos.y - tileCenter.y;
                    float d_c = std::sqrt(dx_c * dx_c + dy_c * dy_c);
                    
                    bool hit = false;
                    if (cellType == 3) {
                        if (d_c < 0.16f) hit = true;
                    } else if (cellType == 5) {
                        float radius = 0.22f;
                        if (cx == static_cast<int>(portalGridPos.x) && cy == static_cast<int>(portalGridPos.y)) {
                            radius = 0.38f;
                        }
                        if (d_c < radius) hit = true;
                    } else {
                        hit = true; // solid wall
                    }
                    if (hit) {
                        hasLineOfSight = false;
                        break;
                    }
                }
            }
        }

        // Detect player (Lantern ON increases detection range - reduced for fairness)
        float detectRange = playerLantern ? 6.0f : 2.2f;

        bool isHiding = player->isHiding;

        if (isHiding) {
            // Move back away from the player's actual position when player hides
            if (dist > 0.05f) {
                sf::Vector2f dir(-dx / dist, -dy / dist); // Move away
                gridPosition += dir * speed * dt;
                
                // Wrap ghost coordinates
                gridPosition.x = IsometricMath::wrapCoord(gridPosition.x, static_cast<float>(gridW));
                gridPosition.y = IsometricMath::wrapCoord(gridPosition.y, static_cast<float>(gridH));

                sf::Vector2f screenDir = IsometricMath::cartesianToIsometric(dir);
                float screenAngle = std::atan2(screenDir.y, screenDir.x) * 180.f / 3.14159265f;
                if (screenAngle < 0.f) screenAngle += 360.f;
                int angleIdx = static_cast<int>(std::round(screenAngle / 45.f)) % 8;
                const int angleToDir[8] = { 1, 2, 3, 4, 5, 6, 7, 0 };
                currentDir = angleToDir[angleIdx];
            }
            walkAnimator.update(dt, currentDir);
            int frame = walkAnimator.getCurrentFrame();
            updateTexture(*runTextures[currentDir][frame]);
        }
        else if (dist < detectRange && hasLineOfSight) {
            // Chase
            float dynamicSpeed = speed + 0.6f * (playerFear / 100.f);
            if (dist > 0.05f) {
                sf::Vector2f dir(dx / dist, dy / dist);
                gridPosition += dir * dynamicSpeed * dt;
                
                // Wrap ghost coordinates
                gridPosition.x = IsometricMath::wrapCoord(gridPosition.x, static_cast<float>(gridW));
                gridPosition.y = IsometricMath::wrapCoord(gridPosition.y, static_cast<float>(gridH));

                // Set facing direction
                float angle = std::atan2(dir.y, dir.x) * 180.f / 3.14159265f;
                sf::Vector2f screenDir = IsometricMath::cartesianToIsometric(dir);
                float screenAngle = std::atan2(screenDir.y, screenDir.x) * 180.f / 3.14159265f;
                if (screenAngle < 0.f) screenAngle += 360.f;
                int angleIdx = static_cast<int>(std::round(screenAngle / 45.f)) % 8;
                const int angleToDir[8] = { 1, 2, 3, 4, 5, 6, 7, 0 };
                currentDir = angleToDir[angleIdx];
            }

            // Animate
            walkAnimator.update(dt, currentDir);
            int frame = walkAnimator.getCurrentFrame();
            updateTexture(*runTextures[currentDir][frame]);
        } else {
            // Idle wander / stand
            walkAnimator.reset();
            updateTexture(*idleTextures[currentDir]);
        }

        // Fade in
        if (alpha < 140.f) {
            alpha += 90.f * dt;
            if (alpha > 140.f) alpha = 140.f;
        }

        // Jump-scare attack (disabled while player is hiding)
        if (dist < 1.0f && !player->isHiding) {
            player->fear += 40.f;
            screamSound.setPitch(1.8f + 0.3f * (std::rand() % 100 / 100.f));
            screamSound.setVolume(90.f);
            screamSound.play();
            spawn(playerGridPos, gridW, gridH); // Teleport away
            spawnCooldown = 12.f; // 12 seconds of safety cooldown before spawning back
        }

        float flickerAlpha = alpha + 20.f * std::sin(totalTime * 20.f);
        flickerAlpha = std::clamp(flickerAlpha, 0.f, 255.f);
        sprite.setColor(sf::Color(220, 20, 20, static_cast<sf::Uint8>(flickerAlpha)));

        // Position screen
        sf::Vector2f tileCenter = IsometricMath::cartesianToIsometric(gridPosition);
        float hoverOffset = 6.f * std::sin(totalTime * 3.f);
        screenPosition = tileCenter + sf::Vector2f(0.f, 112.f + hoverOffset);
        sprite.setPosition(screenPosition);
    }
};

// Weeping Seeker Ghost (Moves only when player is not observing it)
class WeepingGhostEntity : public Entity {
public:
    float speed = 1.8f;
    float alpha = 0.f;
    bool active = true;
    bool isFrozen = false;
    float spawnCooldown = 8.f; // Delay before weeping ghost can chase again (safety buffer at start)
    sf::Texture* runTextures[8][10];
    sf::Texture* idleTextures[8];
    int currentDir = 0;
    Animator walkAnimator;

    WeepingGhostEntity(TextureManager& texManager) : walkAnimator(0, 0, 10, 1.f / 15.f) {
        for (int d = 0; d < 8; ++d) {
            std::string idlePath = "assets/kenney_isometricDungeon/Characters/Male/Male_" + std::to_string(d) + "_Idle0.png";
            idleTextures[d] = &texManager.getTexture("char_male_idle_" + std::to_string(d), idlePath);
            for (int f = 0; f < 10; ++f) {
                std::string runPath = "assets/kenney_isometricDungeon/Characters/Male/Male_" + std::to_string(d) + "_Run" + std::to_string(f) + ".png";
                runTextures[d][f] = &texManager.getTexture("char_male_run_" + std::to_string(d) + "_" + std::to_string(f), runPath);
            }
        }
        updateTexture(*idleTextures[0]);
        sprite.setScale(0.25f, 0.25f);
        sprite.setColor(sf::Color(180, 210, 255, 120)); // Ghostly blue-white
    }

    void updateTexture(const sf::Texture& tex) {
        sprite.setTexture(tex);
        sf::FloatRect bounds = sprite.getLocalBounds();
        sprite.setOrigin(bounds.width / 2.f, bounds.height);
    }

    void update(float dt) override {}

    void spawn(sf::Vector2f playerGridPos, int gridW, int gridH) {
        int sectorX = static_cast<int>(std::floor(playerGridPos.x / gridW));
        int sectorY = static_cast<int>(std::floor(playerGridPos.y / gridH));
        for (int i = 0; i < 30; ++i) {
            float rx = std::rand() % gridW;
            float ry = std::rand() % gridH;
            float absX = sectorX * gridW + rx + 0.5f;
            float absY = sectorY * gridH + ry + 0.5f;
            float dist = std::sqrt((absX - playerGridPos.x)*(absX - playerGridPos.x) + (absY - playerGridPos.y)*(absY - playerGridPos.y));
            if (dist > 8.f && dist < 15.f) {
                gridPosition = sf::Vector2f(absX, absY);
                alpha = 0.f;
                break;
            }
        }
    }

    void updateGhost(float dt, sf::Vector2f playerGridPos, sf::Vector2f playerFacing, bool playerLantern, const std::vector<std::vector<int>>& grid, int gridW, int gridH, sf::Sound& screamSound, Player* player, float totalTime, sf::Vector2f portalGridPos) {
        // Handle spawn cooldown (inactive, invisible, doesn't move)
        if (spawnCooldown > 0.f) {
            spawnCooldown -= dt;
            alpha = 0.f;
            sprite.setColor(sf::Color(0, 0, 0, 0));
            return;
        }

        float dx = IsometricMath::deltaPeriodic(playerGridPos.x - gridPosition.x, static_cast<float>(gridW));
        float dy = IsometricMath::deltaPeriodic(playerGridPos.y - gridPosition.y, static_cast<float>(gridH));
        float dist = std::sqrt(dx * dx + dy * dy);

        // Raycast check from ghost to player to verify line-of-sight
        bool hasLineOfSight = true;
        if (dist > 0.05f) {
            sf::Vector2f rayDir(dx / dist, dy / dist);
            for (float step = 0.1f; step < dist; step += 0.1f) {
                sf::Vector2f checkPos = gridPosition + rayDir * step;
                int cx = IsometricMath::wrapIndex(static_cast<int>(std::floor(checkPos.x)), gridW);
                int cy = IsometricMath::wrapIndex(static_cast<int>(std::floor(checkPos.y)), gridH);
                
                int cellType = grid[cx][cy];
                if (cellType != 0 && cellType != 6 && cellType != 7) {
                    sf::Vector2f tileCenter(static_cast<float>(cx) + 0.5f, static_cast<float>(cy) + 0.5f);
                    float dx_c = checkPos.x - tileCenter.x;
                    float dy_c = checkPos.y - tileCenter.y;
                    float d_c = std::sqrt(dx_c * dx_c + dy_c * dy_c);
                    
                    bool hit = false;
                    if (cellType == 3) {
                        if (d_c < 0.16f) hit = true;
                    } else if (cellType == 5) {
                        float radius = 0.22f;
                        if (cx == static_cast<int>(portalGridPos.x) && cy == static_cast<int>(portalGridPos.y)) {
                            radius = 0.38f;
                        }
                        if (d_c < radius) hit = true;
                    } else {
                        hit = true;
                    }
                    if (hit) {
                        hasLineOfSight = false;
                        break;
                    }
                }
            }
        }

        // Check if player is facing this ghost in Cartesian Grid space using toroidal wrap direction vector
        sf::Vector2f wrappedDir(dx, dy);
        float wrappedLen = std::sqrt(wrappedDir.x * wrappedDir.x + wrappedDir.y * wrappedDir.y);
        if (wrappedLen > 0.f) {
            wrappedDir /= wrappedLen;
        }

        float dot = playerFacing.x * wrappedDir.x + playerFacing.y * wrappedDir.y;
        
        // Weeping Ghost detection range (radius of visibility for fairness)
        float detectRange = playerLantern ? 7.0f : 2.5f;

        // Seeker runs away on getting direct hit of flashlight (dot > 0.84f for 65 deg cone, lantern on, within range and line-of-sight)
        bool hitByFlashlight = playerLantern && (dot > 0.84f) && (dist < detectRange) && hasLineOfSight && !player->isHiding;

        isFrozen = hitByFlashlight; // Freeze fear build-up externally

        bool isHiding = player->isHiding;

        if (isHiding) {
            // Move back away from player actual position when player hides
            if (dist > 0.05f) {
                sf::Vector2f dir(-dx / dist, -dy / dist);
                gridPosition += dir * speed * dt;
                
                // Wrap ghost coordinates
                gridPosition.x = IsometricMath::wrapCoord(gridPosition.x, static_cast<float>(gridW));
                gridPosition.y = IsometricMath::wrapCoord(gridPosition.y, static_cast<float>(gridH));

                sf::Vector2f screenDir = IsometricMath::cartesianToIsometric(dir);
                float screenAngle = std::atan2(screenDir.y, screenDir.x) * 180.f / 3.14159265f;
                if (screenAngle < 0.f) screenAngle += 360.f;
                int angleIdx = static_cast<int>(std::round(screenAngle / 45.f)) % 8;
                const int angleToDir[8] = { 1, 2, 3, 4, 5, 6, 7, 0 };
                currentDir = angleToDir[angleIdx];
            }
            walkAnimator.update(dt, currentDir);
            int frame = walkAnimator.getCurrentFrame();
            updateTexture(*runTextures[currentDir][frame]);
        }
        else if (hitByFlashlight) {
            // Seeker runs away rapidly in the opposite direction
            if (dist > 0.05f) {
                sf::Vector2f dir(-dx / dist, -dy / dist); // Move away
                gridPosition += dir * speed * 1.5f * dt; // 1.5x speed escape
                
                // Wrap ghost coordinates
                gridPosition.x = IsometricMath::wrapCoord(gridPosition.x, static_cast<float>(gridW));
                gridPosition.y = IsometricMath::wrapCoord(gridPosition.y, static_cast<float>(gridH));

                sf::Vector2f screenDir = IsometricMath::cartesianToIsometric(dir);
                float screenAngle = std::atan2(screenDir.y, screenDir.x) * 180.f / 3.14159265f;
                if (screenAngle < 0.f) screenAngle += 360.f;
                int angleIdx = static_cast<int>(std::round(screenAngle / 45.f)) % 8;
                const int angleToDir[8] = { 1, 2, 3, 4, 5, 6, 7, 0 };
                currentDir = angleToDir[angleIdx];
            }
            walkAnimator.update(dt, currentDir);
            int frame = walkAnimator.getCurrentFrame();
            updateTexture(*runTextures[currentDir][frame]);
        }
        else if (!isFrozen && dist < detectRange && hasLineOfSight) {
            // Chase rapidly
            if (dist > 0.05f) {
                sf::Vector2f dir(dx / dist, dy / dist);
                gridPosition += dir * speed * dt;
                
                // Wrap ghost coordinates
                gridPosition.x = IsometricMath::wrapCoord(gridPosition.x, static_cast<float>(gridW));
                gridPosition.y = IsometricMath::wrapCoord(gridPosition.y, static_cast<float>(gridH));

                sf::Vector2f screenDir = IsometricMath::cartesianToIsometric(dir);
                float screenAngle = std::atan2(screenDir.y, screenDir.x) * 180.f / 3.14159265f;
                if (screenAngle < 0.f) screenAngle += 360.f;
                int angleIdx = static_cast<int>(std::round(screenAngle / 45.f)) % 8;
                const int angleToDir[8] = { 1, 2, 3, 4, 5, 6, 7, 0 };
                currentDir = angleToDir[angleIdx];
            }
            walkAnimator.update(dt, currentDir);
            int frame = walkAnimator.getCurrentFrame();
            updateTexture(*runTextures[currentDir][frame]);
        } else {
            // Stand completely frozen covering face
            walkAnimator.reset();
            updateTexture(*idleTextures[currentDir]);
        }

        // Fade in
        if (alpha < 130.f) {
            alpha += 90.f * dt;
            if (alpha > 130.f) alpha = 130.f;
        }

        // Jump-scare attack (disabled while player is hiding)
        if (dist < 1.0f && !player->isHiding) {
            player->fear += 45.f;
            screamSound.setPitch(2.1f + 0.4f * (std::rand() % 100 / 100.f));
            screamSound.setVolume(90.f);
            screamSound.play();
            spawn(playerGridPos, gridW, gridH); // Teleport away
            spawnCooldown = 15.f; // 15 seconds of safety cooldown before spawning back
        }

        float flickerAlpha = alpha + 15.f * std::sin(totalTime * 30.f);
        if (isFrozen) flickerAlpha = alpha; // Don't flicker when frozen under observation
        flickerAlpha = std::clamp(flickerAlpha, 0.f, 255.f);
        sprite.setColor(sf::Color(180, 210, 255, static_cast<sf::Uint8>(flickerAlpha)));

        // Position screen
        sf::Vector2f tileCenter = IsometricMath::cartesianToIsometric(gridPosition);
        float floatOffset = isFrozen ? 0.f : 5.f * std::sin(totalTime * 5.f);
        screenPosition = tileCenter + sf::Vector2f(0.f, 112.f + floatOffset);
        sprite.setPosition(screenPosition);
    }
};
