#include "scene.hpp"

#include "component/info_component.hpp"
#include "component/transform_component.hpp"

Scene::Scene() : _scenePanel{}
{
    ActiveScene = this;

    auto ent = Registry.create();
    Registry.emplace<InfoComponent>(ent);
    Registry.emplace<TransformComponent>(ent);

    auto n2 = Registry.create();
    Registry.emplace<InfoComponent>(n2);
    Registry.emplace<TransformComponent>(n2);
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
