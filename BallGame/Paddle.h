#pragma once
#include <glm/glm.hpp>

class Paddle {
public:
    glm::vec2 position;
    glm::vec2 size;
    float speed;

    Paddle(glm::vec2 pos, glm::vec2 size, float speed);

    void moveUp(float dt);
    void moveDown(float dt);
};
