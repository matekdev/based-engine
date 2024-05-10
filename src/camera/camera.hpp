#pragma once

#include "render/shader.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <GLFW/glfw3.h>

class Camera
{
public:
    inline static Camera *Instance;

    Camera();

    glm::vec3 GetPosition();
    glm::mat4 GetViewMatrix();
    glm::mat4 GetProjectionMatrix();
    glm::mat4 GetViewProjectionMatrix();
    bool IsMouseLocked();

    void Update(float width, float height);
    void Input(float width, float height, GLFWwindow *window, bool isWindowHovered);

private:
    const float VELOCITY_DECAY = 0.97f;
    const float SENSITIVITY = 75.0f;
    const glm::vec3 UP = glm::vec3(0.0f, 1.0f, 0.0f);

    glm::mat4 _viewMatrix = glm::mat4(0.0f);
    glm::mat4 _projectionMatrix = glm::mat4(0.0f);
    glm::vec3 _orientation = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 _position = glm::vec3(0.0f, 0.0f, 2.0f);
    glm::vec3 _velocity = glm::vec3(0.0f, 0.0f, 0.0f);
    bool _isMouseLocked = false;

    void MouseMovement(float width, float height, GLFWwindow *window);
    void KeyboardMovement(GLFWwindow *window);
};