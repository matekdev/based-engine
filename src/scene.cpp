#include "scene.hpp"

#include <GLFW/glfw3.h>

Scene::Scene() : _camera(Camera()), _model(ModelComponent()), _vertexShader(L"shaders/model.vs.hlsl"), _pixelShader(L"shaders/model.ps.hlsl")
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

    _vertexShader.Bind();
    _pixelShader.Bind();
    _model.Render();
}

void Scene::CalculateDeltaTime()
{
    auto currentFrame = glfwGetTime();
    _deltaTime = currentFrame - _previousFrameTime;
    _previousFrameTime = currentFrame;
}
