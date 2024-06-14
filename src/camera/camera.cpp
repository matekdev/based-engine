#include "camera.hpp"

#include "scene.hpp"
#include "render/renderer.hpp"
#include "ui/scene_panel.hpp"

#include <glm/gtc/matrix_transform.hpp>

Camera::Camera() : _cameraMatrixBuffer(ConstantBuffer(ConstantType::CAMERA_MATRIX, ShaderStage::VERTEX_SHADER, CameraMatrixBuffer{}))
{
}

void Camera::Update()
{
    _velocity *= VELOCITY_DECAY;
    _position += _velocity * Scene::ActiveScene->GetDeltaTime();

    _viewMatrix = glm::lookAt(_position, _position + _orientation, UP);
    _projectionMatrix = glm::perspective(glm::radians(90.0f), ScenePanel::GetWidth() / ScenePanel::GetHeight(), 0.01f, 100.0f);

    KeyboardMovement();

    _cameraMatrixBuffer.Update(CameraMatrixBuffer{_projectionMatrix * _viewMatrix});
    _cameraMatrixBuffer.Bind();
}

void Camera::KeyboardMovement()
{
    auto window = Renderer::GetNativeWindow();
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
