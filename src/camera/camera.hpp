#pragma once

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

class Camera
{
public:
    glm::mat4 GetViewProjectionMatrix();

    void Update(float width, float height, GLFWwindow *window);

private:
    const float VELOCITY_DECAY = 0.97f;
    const float SENSITIVITY = 75.0f;
    const glm::vec3 UP = glm::vec3(0.0f, 1.0f, 0.0);

    glm::mat4 _viewMatrix = glm::mat4(0.0f);
    glm::mat4 _projectionMatrix = glm::mat4(0.0f);
    glm::vec3 _orientation = glm::vec3(0.0f);
    glm::vec3 _position = glm::vec3(0.0f);
    glm::vec3 _velocity = glm::vec3(0.0f);

    void KeyboardMovement(GLFWwindow *window);
};
