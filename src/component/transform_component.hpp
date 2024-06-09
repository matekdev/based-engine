#pragma once

#include <glm/glm.hpp>

class TransformComponent
{
public:
    glm::vec3 Position = glm::vec3(0.0f);
    glm::vec3 Rotation = glm::vec3(0.0f);
    glm::vec3 Scale = glm::vec3(1.0f);

    glm::mat4 GetTransform();
    glm::vec3 GetDirection();
};
