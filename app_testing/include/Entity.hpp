#pragma once
#include <SFML/Graphics.hpp>

class Entity {
public:
    sf::Sprite sprite;
    sf::Vector2f gridPosition;   // Cartesian grid position (e.g. 5.5, 4.2)
    sf::Vector2f screenPosition; // Projected screen coordinates

    virtual ~Entity() = default;

    // Entity update step (overridden by dynamic entities)
    virtual void update(float dt) = 0;

    // Z-sorting depth key based on the screen Y ground contact point.
    // Entities with a larger screen Y (closer to the bottom) are drawn last.
    float getDepth() const {
        return screenPosition.y;
    }
};
