#include "scene.hpp"

#include "game_object/game_object.hpp"

Scene::Scene() : _scenePanel{}
{
    ActiveScene = this;
}

void Scene::CreateGameObject()
{
    GameObject gameObject = GameObject(_registry.create(), this);
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
