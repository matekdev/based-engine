#include "scene.hpp"

#include "component/info_component.hpp"
#include "component/transform_component.hpp"
#include "component/light_component.hpp"
#include "component/model/model_component.hpp"

Scene::Scene() : _camera(Camera()),
                 _frameBuffer(FrameBuffer()),
                 _modelShader(Shader("shaders/model.vert", "shaders/model.frag")),
                 _lightShader(Shader("shaders/model.vert", "shaders/light.frag"))
{
    ActiveScene = this;
}

void Scene::CreateNewEntity()
{
    auto ent = Registry.create();
    Registry.emplace<InfoComponent>(ent);
    Registry.emplace<TransformComponent>(ent);
    SelectedEntity = ent;
}

void Scene::Resize(float width, float height)
{
    _frameBuffer.CreateBuffer(width, height);
}

void Scene::Render(GLFWwindow *window)
{
    CalculateDeltaTime();

    _frameBuffer.Bind();

    glClearColor(0.31f, 0.41f, 0.46f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    auto modelGroup = Scene::ActiveScene->Registry.view<ModelComponent>();
    for (auto entity : modelGroup)
    {
        auto &model = modelGroup.get<ModelComponent>(entity);
        model.Render(_modelShader);
    }

    auto lightGroup = Scene::ActiveScene->Registry.view<LightComponent, TransformComponent>();
    for (auto entity : lightGroup)
    {
        auto &light = lightGroup.get<LightComponent>(entity);
        light.Render(_lightShader);

        auto &transform = lightGroup.get<TransformComponent>(entity);
        _modelShader.SetVec3(Shader::LIGHT_POSITION, transform.Position);
        _modelShader.SetVec3(Shader::LIGHT_COLOR, light.Color);
        _modelShader.SetVec3(Shader::CAMERA_POSITION, Scene::ActiveScene->GetCamera().GetPosition());
    }

    _frameBuffer.Unbind();
}

Camera &Scene::GetCamera()
{
    return _camera;
}

GLuint Scene::GetRenderTextureId()
{
    return _frameBuffer.GetTextureId();
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
