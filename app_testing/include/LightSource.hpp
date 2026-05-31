#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>

class LightSource {
public:
    sf::Vector2f position; // World coordinate position
    sf::Vector2f gridPosition; // Grid coordinate position for raycasting
    float radius;
    sf::Sprite sprite;

    LightSource(sf::Vector2f pos, float rad, const sf::Texture& lightTexture, sf::Vector2f gridPos = sf::Vector2f(0.f, 0.f)) 
        : position(pos), gridPosition(gridPos), radius(rad) {
        sprite.setTexture(lightTexture);
        sf::FloatRect bounds = sprite.getLocalBounds();
        sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
        updateScale();
        sprite.setPosition(position);
    }

    void updateScale() {
        sf::FloatRect bounds = sprite.getLocalBounds();
        if (bounds.width > 0.f && bounds.height > 0.f) {
            sprite.setScale((radius * 2.f) / bounds.width, (radius * 2.f) / bounds.height);
        }
    }

    void setRadius(float rad) {
        radius = rad;
        updateScale();
    }

    void setPosition(sf::Vector2f pos) {
        position = pos;
        sprite.setPosition(position);
    }

    void draw(sf::RenderTarget& target) {
        target.draw(sprite, sf::BlendAdd);
    }
};

// Procedurally generate a radial light texture with quadratic falloff
inline sf::Texture createLightTexture(int radius) {
    sf::Image img;
    img.create(radius * 2, radius * 2);
    sf::Vector2f center(radius, radius);

    for (int x = 0; x < radius * 2; ++x) {
        for (int y = 0; y < radius * 2; ++y) {
            sf::Vector2f pos(x, y);
            float dist = std::sqrt((pos.x - center.x) * (pos.x - center.x) + (pos.y - center.y) * (pos.y - center.y));
            if (dist < radius) {
                float factor = 1.f - (dist / radius);
                float alpha = factor * factor * 255.f;
                img.setPixel(x, y, sf::Color(255, 210, 150, static_cast<sf::Uint8>(alpha)));
            } else {
                img.setPixel(x, y, sf::Color(0, 0, 0, 0));
            }
        }
    }
    sf::Texture tex;
    tex.loadFromImage(img);
    return tex;
}

// Procedurally generate a directional cone light texture with edge falloff and a small center ambient glow
inline sf::Texture createFlashlightConeTexture(int radius, float coneAngleDegrees) {
    sf::Image img;
    img.create(radius * 2, radius * 2);
    sf::Vector2f center(radius, radius);
    float halfCone = coneAngleDegrees / 2.f;

    for (int x = 0; x < radius * 2; ++x) {
        for (int y = 0; y < radius * 2; ++y) {
            float dx = x - center.x;
            float dy = y - center.y;
            float dist = std::sqrt(dx * dx + dy * dy);
            
            if (dist < radius) {
                // Flashlight beam factor (pointing to +X / 0 degrees default)
                float angle = std::atan2(dy, dx) * 180.f / 3.14159265f;
                if (angle < -180.f) angle += 360.f;
                if (angle > 180.f) angle -= 360.f;
                
                float angleFactor = 0.f;
                if (std::abs(angle) <= halfCone) {
                    angleFactor = 1.f - (std::abs(angle) / halfCone);
                }
                
                float distFactor = 1.f - (dist / radius);
                float beamFactor = distFactor * distFactor * angleFactor;
                
                // Small ambient glow around player so they can see their immediate vicinity
                float ambientRadius = radius * 0.22f;
                float ambientFactor = 0.f;
                if (dist < ambientRadius) {
                    ambientFactor = 0.35f * (1.f - (dist / ambientRadius)) * (1.f - (dist / ambientRadius));
                }
                
                float finalFactor = std::max(beamFactor, ambientFactor);
                float alpha = finalFactor * 255.f;
                
                img.setPixel(x, y, sf::Color(255, 225, 175, static_cast<sf::Uint8>(alpha)));
            } else {
                img.setPixel(x, y, sf::Color(0, 0, 0, 0));
            }
        }
    }
    sf::Texture tex;
    tex.loadFromImage(img);
    return tex;
}

