#!/bin/bash

# Exit on any error
set -e

echo "========================================="
echo "Downloading Game Assets from GitHub..."
echo "========================================="

# Create assets folder if it doesn't exist
mkdir -p assets

# Download Player Sprite (Eagle Ship from SFML Book)
echo "Downloading Player Sprite..."
curl -L -s -o assets/player.png "https://raw.githubusercontent.com/SFML/SFML-Game-Development-Book/master/02_Resources/Media/Textures/Eagle.png"

# Download Laser Sound Effect (from SuperTux)
echo "Downloading Laser Sound Effect..."
if ! curl -L -s -f -o assets/laser.wav "https://raw.githubusercontent.com/SuperTux/supertux/main/data/sounds/bigjump.wav"; then
    echo "Fallback to master branch for sound..."
    curl -L -s -f -o assets/laser.wav "https://raw.githubusercontent.com/SuperTux/supertux/master/data/sounds/bigjump.wav"
fi

# Download Background Music (from SuperTux)
echo "Downloading Background Music..."
if ! curl -L -s -f -o assets/music.ogg "https://raw.githubusercontent.com/SuperTux/supertux/main/data/music/bonuscave.ogg"; then
    echo "Fallback to master branch for music..."
    curl -L -s -f -o assets/music.ogg "https://raw.githubusercontent.com/SuperTux/supertux/master/data/music/bonuscave.ogg"
fi

echo "-----------------------------------------"
echo "Assets downloaded successfully!"
ls -la assets
echo "========================================="
