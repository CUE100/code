#pragma once
#include <SFML/Graphics.hpp>

class Animator {
private:
    float frameTime;
    float elapsedTime;
    int currentFrame;
    int frameCount;
    sf::IntRect frameRect;
    int frameWidth;
    int frameHeight;

public:
    Animator(int width, int height, int frameCount, float switchTime)
        : frameWidth(width), frameHeight(height), frameCount(frameCount), frameTime(switchTime), elapsedTime(0.f), currentFrame(0) {
        frameRect.width = width;
        frameRect.height = height;
        frameRect.left = 0;
        frameRect.top = 0;
    }

    void update(float deltaTime, int row) {
        elapsedTime += deltaTime;
        if (elapsedTime >= frameTime) {
            elapsedTime -= frameTime;
            currentFrame = (currentFrame + 1) % frameCount;
        }
        frameRect.left = currentFrame * frameWidth;
        frameRect.top = row * frameHeight;
    }

    const sf::IntRect& getFrameRect() const {
        return frameRect;
    }

    int getCurrentFrame() const {
        return currentFrame;
    }

    void setFrameCount(int count) {
        frameCount = count;
    }

    void setFrameTime(float time) {
        frameTime = time;
    }

    void reset() {
        currentFrame = 0;
        elapsedTime = 0.f;
    }
};
