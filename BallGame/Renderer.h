#pragma once
#include <glm/glm.hpp>
#include "Shader.h"

class Renderer {
public:
    Renderer();
    void drawRectangle(glm::vec2 position, glm::vec2 size, glm::vec3 color);
    void drawCircle(glm::vec2 center, float radius, glm::vec3 color);
};
