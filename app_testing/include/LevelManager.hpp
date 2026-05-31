#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <SFML/System/Vector2.hpp>

struct LevelData {
    std::vector<std::vector<int>> grid;
    sf::Vector2f playerSpawn;
    std::vector<sf::Vector2f> pillarSpawns;
    sf::Vector2f genASpawn;
    sf::Vector2f genBSpawn;
    sf::Vector2f genCSpawn;
    sf::Vector2f portalSpawn;

    // Custom asset spawns
    std::vector<sf::Vector2f> houseWallSpawns;
    std::vector<sf::Vector2f> houseWallDoorSpawns;
    std::vector<sf::Vector2f> houseWallWindowSpawns;
    std::vector<sf::Vector2f> houseWallHoleSpawns;
    std::vector<sf::Vector2f> farmFenceSpawns;
    std::vector<sf::Vector2f> farmCropSpawns;
    std::vector<sf::Vector2f> farmHaySpawns;
};

class LevelManager {
public:
    static LevelData loadLevel(int levelNumber) {
        LevelData data;

        // Visual layout representing a 30x30 level
        // ' ' = Void (2)
        // '.' = Stone Floor (0)
        // '>' = Dungeon Right wall (1)
        // '<' = Dungeon Left wall (4)
        // 'X' = Pillar collision block (3)
        // 'P' = Player spawn (0)
        // 'A', 'B', 'C' = Generators (0)
        // 'O' = Escape Portal (0)
        // 'H' = House wooden floor Planks (6)
        // 'F' = Farm dirt lane Floor (7)
        // 'w' = House planks underneath, Wood Wall on top (8)
        // 'd' = House planks underneath, Wood Door on top (6)
        // 'v' = House planks underneath, Wood Window on top (8)
        // 'h' = House planks underneath, Broken Wall entryway on top (6 - walkable)
        // 'f' = Farm dirt underneath, Fence on top (9)
        // 'c' = Farm dirt underneath, Corn crop on top (7)
        // 'y' = Farm dirt underneath, Hay bale on top (9)
        std::vector<std::string> layout;

        if (levelNumber == 1) {
            layout = {
                ">>>>>>..>>>>>>>>>>>>>>>>>>>>>>",
                "<..................< FFF fffff",
                "<....X...A....X....< FFF fcccf",
                "<..................< FFF fcccf",
                "<..................< FFF fcccf",
                "<..................< FFF fffff",
                "...................  FFF      ",
                "...................  FFF      ",
                "<..................< FFF fffff",
                "<....O....P........< FFF fcccf",
                "<..................< FFF fcccf",
                "<....X...B....X....< FFF fcccf",
                "<..................< FFF fffff",
                "<..................< FFF      ",
                "<..................< FFF fffff",
                "...................  FFF fy yy",
                "...................  FFF fy yy",
                "<...C..............< FFF fffff",
                "<..................< FFF      ",
                ">>>>>>..>>>>>>>>>>>> FFF fffff",
                "   .......           FFF fcccf",
                "   .......           FFF fcccf",
                "   wwwwdwwww         FFF fcccf",
                "   wHHHHHHHw         FFF fffff",
                "   wHHHHHHHw                  ",
                "   hHHHHHHHv                  ",
                "   wHHHHHHHw                  ",
                "   wHHHdHHHw                  ",
                "   wwwwwwwww                  ",
                ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>"
            };
        } else {
            // Default fallback level
            layout = {
                ">>>>>>>>>>",
                "<P.......<",
                ">>>>>>>>>>"
            };
        }

        int rows = static_cast<int>(layout.size());
        int cols = static_cast<int>(layout[0].size());

        // Initialize grid: columns x rows
        data.grid.resize(cols, std::vector<int>(rows, 2));

        for (int y = 0; y < rows; ++y) {
            for (int x = 0; x < cols; ++x) {
                // Bounds safety check
                if (x >= static_cast<int>(layout[y].size())) {
                    data.grid[x][y] = 2; // Default to void
                    continue;
                }

                char c = layout[y][x];

                switch (c) {
                    case ' ':
                        data.grid[x][y] = 2; // Abyss/Void
                        break;
                    case '.':
                        data.grid[x][y] = 0; // Floor
                        break;
                    case '>':
                        data.grid[x][y] = 1; // Right-facing Wall
                        break;
                    case '<':
                        data.grid[x][y] = 4; // Left-facing Wall
                        break;
                    case 'X':
                        data.grid[x][y] = 3; // Floor underneath with Pillar collision block
                        data.pillarSpawns.push_back(sf::Vector2f(static_cast<float>(x), static_cast<float>(y)));
                        break;
                    case 'P':
                        data.grid[x][y] = 0; // Floor underneath player spawn
                        data.playerSpawn = sf::Vector2f(static_cast<float>(x), static_cast<float>(y));
                        break;
                    case 'A':
                        data.grid[x][y] = 0; // Floor underneath
                        data.genASpawn = sf::Vector2f(static_cast<float>(x), static_cast<float>(y));
                        break;
                    case 'B':
                        data.grid[x][y] = 0; // Floor underneath
                        data.genBSpawn = sf::Vector2f(static_cast<float>(x), static_cast<float>(y));
                        break;
                    case 'C':
                        data.grid[x][y] = 0; // Floor underneath
                        data.genCSpawn = sf::Vector2f(static_cast<float>(x), static_cast<float>(y));
                        break;
                    case 'O':
                        data.grid[x][y] = 0; // Floor underneath
                        data.portalSpawn = sf::Vector2f(static_cast<float>(x), static_cast<float>(y));
                        break;
                    case 'H':
                        data.grid[x][y] = 6; // Planks
                        break;
                    case 'F':
                        data.grid[x][y] = 7; // Farmland
                        break;
                    case 'w':
                        data.grid[x][y] = 8; // Solid planks
                        data.houseWallSpawns.push_back(sf::Vector2f(static_cast<float>(x), static_cast<float>(y)));
                        break;
                    case 'd':
                        data.grid[x][y] = 6; // Walkable planks
                        data.houseWallDoorSpawns.push_back(sf::Vector2f(static_cast<float>(x), static_cast<float>(y)));
                        break;
                    case 'v':
                        data.grid[x][y] = 8; // Solid planks
                        data.houseWallWindowSpawns.push_back(sf::Vector2f(static_cast<float>(x), static_cast<float>(y)));
                        break;
                    case 'h':
                        data.grid[x][y] = 6; // Walkable planks
                        data.houseWallHoleSpawns.push_back(sf::Vector2f(static_cast<float>(x), static_cast<float>(y)));
                        break;
                    case 'f':
                        data.grid[x][y] = 9; // Solid dirt
                        data.farmFenceSpawns.push_back(sf::Vector2f(static_cast<float>(x), static_cast<float>(y)));
                        break;
                    case 'c':
                        data.grid[x][y] = 7; // Farmland with crop on top
                        data.farmCropSpawns.push_back(sf::Vector2f(static_cast<float>(x), static_cast<float>(y)));
                        break;
                    case 'y':
                        data.grid[x][y] = 9; // Solid dirt
                        data.farmHaySpawns.push_back(sf::Vector2f(static_cast<float>(x), static_cast<float>(y)));
                        break;
                    default:
                        data.grid[x][y] = 2; // Fallback to Void
                        break;
                }
            }
        }

        return data;
    }
};
