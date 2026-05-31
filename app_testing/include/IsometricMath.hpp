#pragma once
#include <SFML/System/Vector2.hpp>
#include <cmath>

namespace IsometricMath {
    // Standard isometric tile dimensions for 2.5D games
    constexpr float TILE_WIDTH = 64.f;
    constexpr float TILE_HEIGHT = 32.f;

    // Convert 2D Grid coordinates to screen coordinates
    inline sf::Vector2f cartesianToIsometric(sf::Vector2f cartesianPos) {
        float x = (cartesianPos.x - cartesianPos.y) * (TILE_WIDTH / 2.f);
        float y = (cartesianPos.x + cartesianPos.y) * (TILE_HEIGHT / 2.f);
        return sf::Vector2f(x, y);
    }

    // Convert screen coordinates back to 2D Grid coordinates
    inline sf::Vector2f isometricToCartesian(sf::Vector2f screenPos) {
        float x = (screenPos.x / (TILE_WIDTH / 2.f) + screenPos.y / (TILE_HEIGHT / 2.f)) / 2.f;
        float y = (screenPos.y / (TILE_HEIGHT / 2.f) - screenPos.x / (TILE_WIDTH / 2.f)) / 2.f;
        return sf::Vector2f(x, y);
    }

    // Helper function for wrapping indices
    inline int wrapIndex(int idx, int maxIdx) {
        int wrapped = idx % maxIdx;
        if (wrapped < 0) wrapped += maxIdx;
        return wrapped;
    }

    // Helper function for wrapping float coordinates
    inline float wrapCoord(float val, float maxVal) {
        float wrapped = std::fmod(val, maxVal);
        if (wrapped < 0.f) wrapped += maxVal;
        return wrapped;
    }

    // Helper function for periodic distance/direction
    inline float deltaPeriodic(float diff, float maxVal) {
        float halfVal = maxVal / 2.f;
        if (diff > halfVal) diff -= maxVal;
        else if (diff < -halfVal) diff += maxVal;
        return diff;
    }
}

