#pragma once
#include "Ball.h"
#include "Paddle.h"
#include "Renderer.h"
#include <GLFW/glfw3.h>

class Game {
public:
    Game(GLFWwindow* window);
    void processInput(float dt);
    void update(float dt);
    void render();
private:
    GLFWwindow* window;
    Ball ball;
    Paddle playerPaddle;
    Paddle aiPaddle;
    Renderer renderer;
    int playerScore;
    int aiScore;
    void checkCollision();
    void reset();
};
