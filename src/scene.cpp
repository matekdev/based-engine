#include "scene.hpp"

#include <GLFW/glfw3.h>

Scene::Scene() : _camera(Camera())
{
    ActiveScene = this;
}

float Scene::GetDeltaTime()
{
    return _deltaTime;
}

Camera &Scene::GetCamera()
{
    return _camera;
}

void Scene::Render()
{
    CalculateDeltaTime();
}

void Scene::CalculateDeltaTime()
{
    auto currentFrame = glfwGetTime();
    _deltaTime = currentFrame - _previousFrameTime;
    _previousFrameTime = currentFrame;
}
