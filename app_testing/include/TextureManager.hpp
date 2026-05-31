#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <string>
#include <memory>
#include <iostream>

class TextureManager {
private:
    std::unordered_map<std::string, std::unique_ptr<sf::Texture>> textures;

public:
    sf::Texture& getTexture(const std::string& name, const std::string& path) {
        auto it = textures.find(name);
        if (it != textures.end()) {
            return *(it->second);
        }

        auto newTexture = std::make_unique<sf::Texture>();
        if (!newTexture->loadFromFile(path)) {
            std::cerr << "Failed to load texture " << path << std::endl;
        }
        textures[name] = std::move(newTexture);
        return *textures[name];
    }
};
