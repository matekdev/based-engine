#pragma once

#include "render/constant_buffer.hpp"

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

class Camera
{
public:
    struct alignas(16) CameraMatrixBuffer
    {
        glm::mat4 ViewMatrix;
        glm::mat4 ProjectionMatrix;
        glm::vec3 Position;
    };

    Camera();

    glm::vec3 GetPosition();
    glm::mat4 GetViewMatrix();
    glm::mat4 GetProjectionMatrix();

    void Update();

private:
    const float FOV = 90.0f;
    const float VELOCITY_DECAY = 0.97f;
    const float SENSITIVITY = 75.0f;
    const glm::vec3 UP = glm::vec3(0.0f, 1.0f, 0.0);

    ConstantBuffer<CameraMatrixBuffer> _cameraMatrixBuffer;

    glm::mat4 _viewMatrix = glm::mat4(0.0f);
    glm::mat4 _projectionMatrix = glm::mat4(0.0f);
    glm::vec3 _orientation = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 _position = glm::vec3(0.0f, 1.0f, 3.0f);
    glm::vec3 _velocity = glm::vec3(0.0f);

    bool _isMouseLocked;

    void Input();
    void KeyboardMovement();
    void MouseMovement();
};
