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
    _velocity *= VELOCITY_DECAY;
    _position += _velocity * Scene::ActiveScene->GetDeltaTime();

    _viewMatrix = glm::lookAt(_position, _position + _orientation, UP);
    _projectionMatrix = glm::perspective(glm::radians(FOV), ScenePanel::GetWidth() / ScenePanel::GetHeight(), 0.01f, 100.0f);

    KeyboardMovement();
    MouseMovement();

    _cameraMatrixBuffer.Update(CameraMatrixBuffer{_viewMatrix, _projectionMatrix});
    _cameraMatrixBuffer.Bind();
}

void Camera::KeyboardMovement()
{
    auto window = GLFWUtil::GetNativeWindow();
    auto movementSpeed = 0.2f;

    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        movementSpeed *= 4;

    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        movementSpeed /= 4;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        _velocity += movementSpeed * _orientation;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        _velocity += movementSpeed * -glm::normalize(glm::cross(_orientation, UP));
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        _velocity += movementSpeed * -_orientation;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        _velocity += movementSpeed * glm::normalize(glm::cross(_orientation, UP));
}

void Camera::MouseMovement()
{
    auto window = GLFWUtil::GetNativeWindow();

    auto isRightClickDown = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS;
    if (!_isMouseLocked)
        _isMouseLocked = isRightClickDown && ScenePanel::IsHovered();

    auto isRightClickReleased = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE;
    if (isRightClickReleased)
        _isMouseLocked = false;

    glfwSetInputMode(window, GLFW_CURSOR, _isMouseLocked ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);

    if (!_isMouseLocked)
        return;

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
