cmake_minimum_required(VERSION 3.10)
project(BallGame)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(external/glfw)

add_executable(BallGame
    src/main.cpp
    src/Game.cpp
    src/Ball.cpp
    src/Paddle.cpp
    src/Renderer.cpp
    src/Shader.cpp
    src/utilities.cpp
