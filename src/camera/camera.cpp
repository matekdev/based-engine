#include "camera.hpp"

#include "scene.hpp"
#include "ui/scene_panel.hpp"
#include "common/glfw_util.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/vector_angle.hpp>

Camera::Camera() : _cameraMatrixBuffer(ConstantBuffer(ConstantType::CAMERA_MATRIX, ShaderStage::VERTEX_SHADER, CameraMatrixBuffer{}))
{
}

glm::vec3 Camera::GetPosition()
{
    return _position;
}

glm::mat4 Camera::GetViewMatrix()
{
    return _viewMatrix;
}

glm::mat4 Camera::GetProjectionMatrix()
{
    return _projectionMatrix;
}

void Camera::Update()
{
    Input();

    _velocity *= VELOCITY_DECAY;
    _position += _velocity * Scene::ActiveScene->GetDeltaTime();

    _viewMatrix = glm::lookAt(_position, _position + _orientation, UP);
    _projectionMatrix = glm::perspective(glm::radians(FOV), ScenePanel::GetWidth() / ScenePanel::GetHeight(), 0.01f, 100.0f);

    _cameraMatrixBuffer.Update(CameraMatrixBuffer{_viewMatrix, _projectionMatrix});
    _cameraMatrixBuffer.Bind();
}

void Camera::Input()
{
    auto window = GLFWUtil::GetNativeWindow();

    auto isRightClickDown = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS;
    if (!_isMouseLocked)
        _isMouseLocked = isRightClickDown && ScenePanel::IsHovered();

    auto isRightClickReleased = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE;
    if (isRightClickReleased)
        _isMouseLocked = false;

    GLFWUtil::SetMouseLock(_isMouseLocked);

    if (!_isMouseLocked)
        return;

    KeyboardMovement();
    MouseMovement();
}

void Camera::KeyboardMovement()
{
    auto window = GLFWUtil::GetNativeWindow();
    auto movementSpeed = 0.2f;

    if (GLFWUtil::IsButtonPressed(GLFW_KEY_LEFT_SHIFT))
        movementSpeed *= 4;

    if (GLFWUtil::IsButtonPressed(GLFW_KEY_LEFT_CONTROL))
        movementSpeed /= 4;

    if (GLFWUtil::IsButtonPressed(GLFW_KEY_W))
        _velocity += movementSpeed * _orientation;
    if (GLFWUtil::IsButtonPressed(GLFW_KEY_A))
        _velocity += movementSpeed * -glm::normalize(glm::cross(_orientation, UP));
    if (GLFWUtil::IsButtonPressed(GLFW_KEY_S))
        _velocity += movementSpeed * -_orientation;
    if (GLFWUtil::IsButtonPressed(GLFW_KEY_D))
        _velocity += movementSpeed * glm::normalize(glm::cross(_orientation, UP));
}

void Camera::MouseMovement()
{
    auto window = GLFWUtil::GetNativeWindow();
    auto windowSize = GLFWUtil::GetWindowSize();
    auto mouse = GLFWUtil::GetMousePosition();
    glfwSetCursorPos(window, (windowSize.x / 2), (windowSize.y / 2));

    float rotationX = SENSITIVITY * (float)(mouse.y - (windowSize.y / 2)) / windowSize.y;
    float rotationY = SENSITIVITY * (float)(mouse.x - (windowSize.x / 2)) / windowSize.x;

    glm::vec3 newOrientation = glm::rotate(_orientation, glm::radians(-rotationX), glm::normalize(glm::cross(_orientation, UP)));

    if (abs(glm::angle(newOrientation, UP) - glm::radians(90.0f)) <= glm::radians(85.0f))
        _orientation = newOrientation;

    _orientation = glm::rotate(_orientation, glm::radians(-rotationY), UP);
}
