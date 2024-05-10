#include "scene.hpp"

Scene::Scene() : _scenePanel()
{
}

void Scene::Render(GLFWwindow *window)
{
    CalculateDeltaTime();

    _scenePanel.Render(window);
    _scenePanel.Input(window);
}

float Scene::GetDeltaTime()
{
    return _deltaTime;
}

void Scene::CalculateDeltaTime()
{
    auto currentFrame = glfwGetTime();
    _deltaTime = currentFrame - _previousFrameTime;
    _previousFrameTime = currentFrame;
}
