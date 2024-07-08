#include "scene.hpp"

#include "components/info_component.hpp"
#include "components/transform_component.hpp"
#include "components/model/model_component.hpp"

#include <GLFW/glfw3.h>

Scene::Scene() : _renderTarget(RenderTarget()), _camera(Camera()), _vertexShader(L"shaders/model.vs.hlsl"), _pixelShader(L"shaders/model.ps.hlsl")
{
    ActiveScene = this;
}

ID3D11ShaderResourceView *Scene::GetShaderResourceView()
{
    return _renderTarget.GetShaderResourceView();
}

Camera &Scene::GetActiveCamera()
{
    return _camera;
}

float Scene::GetDeltaTime()
{
    return _deltaTime;
}

void Scene::CreateNewEntity()
{
    auto ent = Registry.create();
    Registry.emplace<InfoComponent>(ent, ent);
    Registry.emplace<TransformComponent>(ent, ent);
    Registry.emplace<ModelComponent>(ent, ent);
}

void Scene::OnResize()
{
    _renderTarget.Resize();
}

void Scene::Render()
{
    CalculateDeltaTime();

    _renderTarget.Bind();
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

    _camera.Update();
}

void Scene::CalculateDeltaTime()
{
    auto currentFrame = glfwGetTime();
    _deltaTime = currentFrame - _previousFrameTime;
    _previousFrameTime = currentFrame;
}
