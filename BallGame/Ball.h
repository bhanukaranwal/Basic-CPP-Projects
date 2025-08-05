#pragma once
#include <glm/glm.hpp>

class Ball {
public:
    glm::vec2 position;
    glm::vec2 velocity;
    float radius;
    Ball(glm::vec2 pos, float r);

    void update(float dt);
    void reset(glm::vec2 startPos);
};
