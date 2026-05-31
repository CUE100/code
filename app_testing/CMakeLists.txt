cmake_minimum_required(VERSION 3.12)
project(IsometricHorrorDungeon CXX)

# Require C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Hint for Homebrew's SFML 2.6.2 installation on macOS (Apple Silicon)
list(APPEND CMAKE_PREFIX_PATH "/opt/homebrew/opt/sfml@2")

# Find SFML 2.6 components
find_package(SFML 2.6 REQUIRED COMPONENTS graphics window system audio)

# Discover source files
file(GLOB_RECURSE SRC_FILES "src/*.cpp")

# Define target executable
add_executable(isometric-game ${SRC_FILES})

# Include project headers and system-linked SFML headers
target_include_directories(isometric-game PRIVATE include)

# Link SFML libraries
target_link_libraries(isometric-game PRIVATE sfml-graphics sfml-window sfml-system sfml-audio)

# Post-build step: Copy assets directory to the target build directory
# This allows relative assets loading (like "assets/music.ogg") to work.
add_custom_command(TARGET isometric-game POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_CURRENT_SOURCE_DIR}/assets"
    "$<TARGET_FILE_DIR:isometric-game>/assets"
    COMMENT "Copying assets to build output directory..."
)
