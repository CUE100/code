#pragma once
#include <SFML/Graphics.hpp>
#include "Entity.hpp"
#include "IsometricMath.hpp"

// Wall Entity (Z-sorted dynamic wall)
class WallEntity : public Entity {
public:
    WallEntity(const sf::Texture& texture, sf::Vector2f gridPos) {
        sprite.setTexture(texture);
        sf::FloatRect bounds = sprite.getLocalBounds();
        sprite.setOrigin(bounds.width / 2.f, bounds.height);
        sprite.setScale(0.25f, 0.25f);
        gridPosition = gridPos;
        sf::Vector2f tileCenter = IsometricMath::cartesianToIsometric(gridPosition);
        screenPosition = tileCenter + sf::Vector2f(0.f, 112.f);
        sprite.setPosition(screenPosition);
    }
    void update(float dt) override {}
};

// Pillar Obstacle Entity
class PillarEntity : public Entity {
public:
    PillarEntity(const sf::Texture& pillarTex, sf::Vector2f gridPos) {
        sprite.setTexture(pillarTex);
        sf::FloatRect bounds = sprite.getLocalBounds();
        sprite.setOrigin(bounds.width / 2.f, bounds.height);
        sprite.setScale(0.25f, 0.25f);
        gridPosition = gridPos;
        sf::Vector2f tileCenter = IsometricMath::cartesianToIsometric(gridPosition);
        screenPosition = tileCenter + sf::Vector2f(0.f, 112.f);
        sprite.setPosition(screenPosition);
    }
    void update(float dt) override {}
};

// Prop Entity (Obstacles and details)
class PropEntity : public Entity {
public:
    PropEntity(const sf::Texture& texture, sf::Vector2f gridPos, float scale = 0.25f) {
        sprite.setTexture(texture);
        sf::FloatRect bounds = sprite.getLocalBounds();
        sprite.setOrigin(bounds.width / 2.f, bounds.height);
        sprite.setScale(scale, scale);
        gridPosition = gridPos;
        sf::Vector2f tileCenter = IsometricMath::cartesianToIsometric(gridPosition);
        screenPosition = tileCenter + sf::Vector2f(0.f, 112.f);
        sprite.setPosition(screenPosition);
    }
    void update(float dt) override {}
};

// Interactive Prop Entity (Generators, Portal)
class InteractiveEntity : public Entity {
public:
    bool isActive = false;
    sf::Color baseColor;

    InteractiveEntity(const sf::Texture& texture, sf::Vector2f gridPos, sf::Color color) {
        sprite.setTexture(texture);
        sf::FloatRect bounds = sprite.getLocalBounds();
        sprite.setOrigin(bounds.width / 2.f, bounds.height);
        sprite.setScale(0.25f, 0.25f);
        baseColor = color;
        sprite.setColor(baseColor);
        gridPosition = gridPos;
        sf::Vector2f tileCenter = IsometricMath::cartesianToIsometric(gridPosition);
        screenPosition = tileCenter + sf::Vector2f(0.f, 112.f);
        sprite.setPosition(screenPosition);
    }

    void setActive(bool active) {
        isActive = active;
        if (isActive) {
            sprite.setColor(sf::Color(100, 255, 100)); // Glowing green
        } else {
            sprite.setColor(baseColor);
        }
    }

    void update(float dt) override {}
};
