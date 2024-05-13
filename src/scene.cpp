#include "scene.hpp"

#include "component/info_component.hpp"
#include "component/transform_component.hpp"
#include "component/model/model_component.hpp"

Scene::Scene() : _scenePanel{}
{
    ActiveScene = this;

    CreateNewEntity();
    CreateNewEntity();
    CreateNewEntity();
    CreateNewEntity();
}

void Scene::CreateNewEntity()
{
    auto ent = Registry.create();
    Registry.emplace<InfoComponent>(ent);
    Registry.emplace<TransformComponent>(ent);
    Registry.emplace<ModelComponent>(ent, ent);
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

void Scene::OnMouseClick()
{
    _scenePanel.OnMouseClick();
}

void Scene::CalculateDeltaTime()
{
    auto currentFrame = glfwGetTime();
    _deltaTime = currentFrame - _previousFrameTime;
    _previousFrameTime = currentFrame;
}
