#pragma once
#include <vector>
#include <algorithm>
#include <SFML/Graphics.hpp>
#include "Entity.hpp"

class EntityManager {
private:
    std::vector<Entity*> m_entities;

public:
    ~EntityManager() {
        clear();
    }

    // Register a new entity
    void addEntity(Entity* entity) {
        m_entities.push_back(entity);
    }

    // Update all dynamic entities
    void update(float dt) {
        for (auto* entity : m_entities) {
            entity->update(dt);
        }
    }

    // Clean up allocated memory
    void clear() {
        for (auto* entity : m_entities) {
            delete entity;
        }
        m_entities.clear();
    }

    // Sort entities based on depth and render them
    void draw(sf::RenderWindow& window) {
        // Sort using the screen Y coordinate of ground contact (screenPosition.y) in ascending order
        std::sort(m_entities.begin(), m_entities.end(), [](const Entity* a, const Entity* b) {
            return a->getDepth() < b->getDepth();
        });

        // Draw sorted entities from back to front (lowest depth/Y screen position drawn first)
        for (const auto* entity : m_entities) {
            window.draw(entity->sprite);
        }
    }

    // Draw sorted entities shifted by a specific screen space offset (used for seamless looping views)
    void drawWithOffset(sf::RenderWindow& window, sf::Vector2f offset) {
        // Sort before drawing
        std::sort(m_entities.begin(), m_entities.end(), [](const Entity* a, const Entity* b) {
            return a->getDepth() < b->getDepth();
        });

        for (auto* entity : m_entities) {
            sf::Vector2f origPos = entity->sprite.getPosition();
            // Temporarily apply screen offset
            entity->sprite.setPosition(origPos + offset);
            window.draw(entity->sprite);
            // Restore original position
            entity->sprite.setPosition(origPos);
        }
    }

    const std::vector<Entity*>& getEntities() const {
        return m_entities;
    }
};
