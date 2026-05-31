#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include <string>
#include "Entity.hpp"
#include "TextureManager.hpp"
#include "Animator.hpp"
#include "IsometricMath.hpp"

class Player : public Entity {
public:
    float speed = 3.6f; // Base walking speed
    sf::Texture* runTextures[8][10];
    sf::Texture* idleTextures[8];
    int currentDir = 0;
    Animator walkAnimator;

    // Gameplay Metrics
    float fear = 0.f;      // 0 to 100
    float stamina = 100.f; // 0 to 100
    bool lanternOn = true;
    bool fatigued = false;
    bool isRunning = false;
    sf::Vector2f lastMovementDir = sf::Vector2f(0.f, 1.f); // Default facing South

    // Improved controls metrics
    sf::Vector2f velocity = sf::Vector2f(0.f, 0.f);
    bool isHiding = false;

    Player(TextureManager& texManager) : walkAnimator(0, 0, 10, 1.f / 12.f) {
        // Load all 8 directions of run and idle textures
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
    }

    void updateTexture(const sf::Texture& tex) {
        sprite.setTexture(tex);
        sf::FloatRect bounds = sprite.getLocalBounds();
        sprite.setOrigin(bounds.width / 2.f, bounds.height);
    }

    void update(float dt) override {}

    void updateWithCollision(float dt, const std::vector<std::vector<int>>& grid) {
        if (isHiding) return;

        sf::Vector2f inputDir(0.f, 0.f);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            inputDir.y -= 1.f;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
            inputDir.y += 1.f;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            inputDir.x -= 1.f;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            inputDir.x += 1.f;
        }

        // Normalize input direction
        float inputLen = std::sqrt(inputDir.x * inputDir.x + inputDir.y * inputDir.y);
        sf::Vector2f targetVelocity(0.f, 0.f);
        if (inputLen > 0.001f) {
            targetVelocity = inputDir / inputLen;
        }

        // Smooth velocity LERP (snappy response, but adds minor inertia for natural control)
        float lerpSpeed = 12.5f;
        velocity += (targetVelocity - velocity) * lerpSpeed * dt;

        float currentSpeed = speed;
        // Stamina & Running logic
        bool isSprinting = (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift)) && !fatigued;
        
        // Fatigued recovery
        if (fatigued && stamina >= 25.f) {
            fatigued = false;
        }

        float velLen = std::sqrt(velocity.x * velocity.x + velocity.y * velocity.y);
        bool isMoving = (velLen > 0.05f);

        if (isSprinting && isMoving) {
            currentSpeed *= 1.65f;
            stamina -= 22.f * dt;
            if (stamina <= 0.f) {
                stamina = 0.f;
                fatigued = true;
            }
        } else {
            // Recharge stamina
            if (!isMoving) {
                stamina += 18.f * dt; // Faster recharge when standing still
            } else {
                stamina += 10.f * dt; // Slower recharge when walking
            }
            if (stamina > 100.f) stamina = 100.f;
        }

        isRunning = isSprinting && isMoving;

        // Helper function to check if a cell is walkable
        auto isWalkable = [](int cellType) {
            return (cellType == 0 || cellType == 6 || cellType == 7);
        };

        if (isMoving) {
            // Update facing direction using velocity vector
            lastMovementDir = velocity / velLen;

            // Map the screen movement to grid space using the isometric aspect ratio (2:1)
            sf::Vector2f screenVec(velocity.x * 2.f, velocity.y);
            sf::Vector2f dir = IsometricMath::isometricToCartesian(screenVec);
            
            // Normalize grid direction
            float length = std::sqrt(dir.x * dir.x + dir.y * dir.y);
            if (length > 0.001f) {
                dir /= length;
            }

            int gridW = static_cast<int>(grid.size());
            int gridH = static_cast<int>(grid[0].size());
            const float padding = 0.22f; // Character radius padding

            // 1. Test X Movement with corner sliding
            float targetX = gridPosition.x + dir.x * velLen * currentSpeed * dt;
            float checkXVal = targetX + (dir.x > 0.f ? padding : -padding);
            int checkX = IsometricMath::wrapIndex(static_cast<int>(std::floor(checkXVal)), gridW);
            
            int checkY1 = IsometricMath::wrapIndex(static_cast<int>(std::floor(gridPosition.y - padding)), gridH);
            int checkY2 = IsometricMath::wrapIndex(static_cast<int>(std::floor(gridPosition.y + padding)), gridH);

            if (isWalkable(grid[checkX][checkY1]) && isWalkable(grid[checkX][checkY2])) {
                gridPosition.x = targetX;
            } else {
                // Squeeze/slide around corners on X block
                bool topBlocked = !isWalkable(grid[checkX][checkY1]);
                bool bottomBlocked = !isWalkable(grid[checkX][checkY2]);
                if (topBlocked && !bottomBlocked) {
                    gridPosition.y += velLen * currentSpeed * dt * 0.5f;
                } else if (!topBlocked && bottomBlocked) {
                    gridPosition.y -= velLen * currentSpeed * dt * 0.5f;
                }
            }

            // 2. Test Y Movement with corner sliding
            float targetY = gridPosition.y + dir.y * velLen * currentSpeed * dt;
            float checkYVal = targetY + (dir.y > 0.f ? padding : -padding);
            int checkYWrapped = IsometricMath::wrapIndex(static_cast<int>(std::floor(checkYVal)), gridH);
            
            int checkX1 = IsometricMath::wrapIndex(static_cast<int>(std::floor(gridPosition.x - padding)), gridW);
            int checkX2 = IsometricMath::wrapIndex(static_cast<int>(std::floor(gridPosition.x + padding)), gridW);

            if (isWalkable(grid[checkX1][checkYWrapped]) && isWalkable(grid[checkX2][checkYWrapped])) {
                gridPosition.y = targetY;
            } else {
                // Squeeze/slide around corners on Y block
                bool leftBlocked = !isWalkable(grid[checkX1][checkYWrapped]);
                bool rightBlocked = !isWalkable(grid[checkX2][checkYWrapped]);
                if (leftBlocked && !rightBlocked) {
                    gridPosition.x += velLen * currentSpeed * dt * 0.5f;
                } else if (!leftBlocked && rightBlocked) {
                    gridPosition.x -= velLen * currentSpeed * dt * 0.5f;
                }
            }

            // Update Direction index based on the screen input angle
            float angle = std::atan2(velocity.y, velocity.x) * 180.f / 3.14159265f;
            if (angle < 0.f) angle += 360.f;
            int angleIdx = static_cast<int>(std::round(angle / 45.f)) % 8;
            
            // Clockwise Kenney sprite mapping starting from East
            const int angleToDir[8] = { 1, 2, 3, 4, 5, 6, 7, 0 };
            currentDir = angleToDir[angleIdx];

            // Update Animation frame
            float frameRate = isSprinting ? 18.f : 12.f;
            walkAnimator.setFrameTime(1.f / frameRate);
            walkAnimator.update(dt, currentDir);
            int animFrame = walkAnimator.getCurrentFrame();
            updateTexture(*runTextures[currentDir][animFrame]);
        } else {
            // Idle animation
            walkAnimator.reset();
            updateTexture(*idleTextures[currentDir]);
        }

        // Calculate screenPosition
        sf::Vector2f tileCenter = IsometricMath::cartesianToIsometric(gridPosition);
        screenPosition = tileCenter + sf::Vector2f(0.f, 112.f); 
        sprite.setPosition(screenPosition);
    }
};
