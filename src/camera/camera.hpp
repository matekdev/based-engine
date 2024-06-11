#pragma once

#include "render/constant_buffer.hpp"

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

class Camera
{
public:
    struct CameraMatrixBuffer
    {
        glm::mat4 Matrix;
    };

    Camera();

    void Update(GLFWwindow *window, float width, float height);

private:
    const float VELOCITY_DECAY = 0.97f;
    const float SENSITIVITY = 75.0f;
    const glm::vec3 UP = glm::vec3(0.0f, 1.0f, 0.0);

    ConstantBuffer<CameraMatrixBuffer> _cameraMatrixBuffer;

    glm::mat4 _viewMatrix = glm::mat4(0.0f);
    glm::mat4 _projectionMatrix = glm::mat4(0.0f);
    glm::vec3 _orientation = glm::vec3(0.0f);
    glm::vec3 _position = glm::vec3(0.0f);
    glm::vec3 _velocity = glm::vec3(0.0f);

    void KeyboardMovement(GLFWwindow *window);
};
