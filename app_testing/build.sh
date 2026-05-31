#!/bin/bash

# Exit on any error
set -e

echo "========================================="
echo "Building 2.5D Isometric Horror Game..."
echo "========================================="

# Output binary name
OUT="isometric-game"

# SFML 2.6.2 Homebrew paths
SFML_INCLUDE="/opt/homebrew/opt/sfml@2/include"
SFML_LIB="/opt/homebrew/opt/sfml@2/lib"

# Check if src/main.cpp exists
if [ ! -f "src/main.cpp" ]; then
    echo "Error: src/main.cpp not found!"
    exit 1
fi

# Compile the project
# Includes the 'include' directory containing IsometricMath, DungeonGenerator, etc.
clang++ -std=c++17 src/main.cpp -o "$OUT" \
    -Iinclude \
    -I"$SFML_INCLUDE" \
    -L"$SFML_LIB" \
    -lsfml-graphics \
    -lsfml-window \
    -lsfml-system \
    -lsfml-audio \
    -Wl,-rpath,"$SFML_LIB"

echo "-----------------------------------------"
echo "Compilation successful!"
echo "To run the game, execute: ./$OUT"
echo "========================================="
