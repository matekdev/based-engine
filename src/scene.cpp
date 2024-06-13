#include "scene.hpp"

#include "component/info_component.hpp"
#include "component/transform_component.hpp"
#include "component/model/model_component.hpp"

#include <GLFW/glfw3.h>

Scene::Scene() : _camera(Camera()), _vertexShader(L"shaders/model.vs.hlsl"), _pixelShader(L"shaders/model.ps.hlsl")
{
    ActiveScene = this;

    CreateNewEntity();
}

float Scene::GetDeltaTime()
{
    return _deltaTime;
}

Camera &Scene::GetCamera()
{
    return _camera;
}

void Scene::CreateNewEntity()
{
    auto ent = Registry.create();
    Registry.emplace<InfoComponent>(ent);
    Registry.emplace<TransformComponent>(ent);
    Registry.emplace<ModelComponent>(ent);
}

void Scene::Render(GLFWwindow *window, int width, int height)
{
    CalculateDeltaTime();

    _vertexShader.Bind();
    _pixelShader.Bind();

    const auto modelGroup = Scene::ActiveScene->Registry.view<ModelComponent, TransformComponent>();
    for (const auto &entity : modelGroup)
    {
        auto &transform = modelGroup.get<TransformComponent>(entity);
        const auto &model = modelGroup.get<ModelComponent>(entity);

        transform.Bind();
        model.Render();
    }

    _camera.Update(window, width, height);
}

void Scene::CalculateDeltaTime()
{
    auto currentFrame = glfwGetTime();
    _deltaTime = currentFrame - _previousFrameTime;
    _previousFrameTime = currentFrame;
}
