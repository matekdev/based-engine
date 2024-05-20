#include "scene.hpp"

#include "component/info_component.hpp"
#include "component/transform_component.hpp"
#include "component/directional_light_component.hpp"
#include "component/point_light_component.hpp"
#include "component/spot_light_component.hpp"
#include "component/model/model_component.hpp"

Scene::Scene() : _camera(Camera()),
                 _frameBuffer(FrameBuffer()),
                 _modelShader(Shader("shaders/model.vert", "shaders/model.frag")),
                 _lightShader(Shader("shaders/model.vert", "shaders/model.frag"))
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

        _modelShader.Bind();
        _modelShader.SetVec3(Shader::MATERIAL_AMBIENT, model.Ambient);
        _modelShader.SetVec3(Shader::MATERIAL_DIFFUSE, model.Diffuse);
        _modelShader.SetVec3(Shader::MATERIAL_SPECULAR, model.Specular);
        _modelShader.SetFloat(Shader::MATERIAL_SHININESS, model.Shininess);

        _modelShader.SetVec3(Shader::CAMERA_POSITION, Camera::Instance->GetPosition());
        _modelShader.SetMat4(Shader::CAMERA_MATRIX, Camera::Instance->GetViewProjectionMatrix());
        _modelShader.SetMat4(Shader::MODEL_MATRIX, Scene::ActiveScene->Registry.get<TransformComponent>(entity).GetTransform());
        _modelShader.SetBool(Shader::HAS_TEXTURES, model.HasTextures());

        model.RenderMesh();
    }

    auto directionalLightGroup = Scene::ActiveScene->Registry.view<DirectionalLightComponent, TransformComponent>();
    _modelShader.Bind();
    _modelShader.SetInt(Shader::DIRECTIONAL_LIGHT_COUNT, directionalLightGroup.size_hint());

    for (auto it = directionalLightGroup.begin(); it != directionalLightGroup.end(); ++it)
    {
        int index = std::distance(directionalLightGroup.begin(), it);
        auto &light = directionalLightGroup.get<DirectionalLightComponent>(*it);
        auto &transform = directionalLightGroup.get<TransformComponent>(*it);

        _modelShader.SetVec3(Shader::Format(Shader::DIRECTIONAL_LIGHTS, Shader::DIRECTION, index), transform.GetDirection());
        _modelShader.SetVec3(Shader::Format(Shader::DIRECTIONAL_LIGHTS, Shader::AMBIENT, index), light.Ambient);
        _modelShader.SetVec3(Shader::Format(Shader::DIRECTIONAL_LIGHTS, Shader::DIFFUSE, index), light.Diffuse);
        _modelShader.SetVec3(Shader::Format(Shader::DIRECTIONAL_LIGHTS, Shader::SPECULAR, index), light.Specular);
    }

    auto pointLightGroup = Scene::ActiveScene->Registry.view<PointLightComponent, TransformComponent>();
    _modelShader.Bind();
    _modelShader.SetInt(Shader::POINT_LIGHT_COUNT, pointLightGroup.size_hint());

    for (auto it = pointLightGroup.begin(); it != pointLightGroup.end(); ++it)
    {
        int index = std::distance(pointLightGroup.begin(), it);
        auto &light = pointLightGroup.get<PointLightComponent>(*it);
        auto &transform = pointLightGroup.get<TransformComponent>(*it);

        _modelShader.SetVec3(Shader::Format(Shader::POINT_LIGHTS, Shader::POSITION, index), transform.Position);
        _modelShader.SetVec3(Shader::Format(Shader::POINT_LIGHTS, Shader::AMBIENT, index), light.Ambient);
        _modelShader.SetVec3(Shader::Format(Shader::POINT_LIGHTS, Shader::DIFFUSE, index), light.Diffuse);
        _modelShader.SetVec3(Shader::Format(Shader::POINT_LIGHTS, Shader::SPECULAR, index), light.Specular);
        _modelShader.SetFloat(Shader::Format(Shader::POINT_LIGHTS, Shader::CONSTANT, index), light.Linear);
        _modelShader.SetFloat(Shader::Format(Shader::POINT_LIGHTS, Shader::LINEAR, index), light.Quadratic);
        _modelShader.SetFloat(Shader::Format(Shader::POINT_LIGHTS, Shader::QUADRATIC, index), light.Quadratic);
    }

    auto spotLightGroup = Scene::ActiveScene->Registry.view<SpotLightComponent, TransformComponent>();
    _modelShader.Bind();
    _modelShader.SetInt(Shader::SPOT_LIGHT_COUNT, spotLightGroup.size_hint());

    for (auto it = spotLightGroup.begin(); it != spotLightGroup.end(); ++it)
    {
        int index = std::distance(spotLightGroup.begin(), it);
        auto &light = spotLightGroup.get<SpotLightComponent>(*it);
        auto &transform = spotLightGroup.get<TransformComponent>(*it);

        _modelShader.SetVec3(Shader::Format(Shader::SPOT_LIGHTS, Shader::POSITION, index), transform.Position);
        _modelShader.SetVec3(Shader::Format(Shader::SPOT_LIGHTS, Shader::DIRECTION, index), transform.GetDirection());
        _modelShader.SetFloat(Shader::Format(Shader::SPOT_LIGHTS, Shader::RADIUS, index), glm::cos(glm::radians(light.Radius)));
        _modelShader.SetFloat(Shader::Format(Shader::SPOT_LIGHTS, Shader::RADIUS, index), glm::cos(glm::radians(light.Radius + 5)));
        _modelShader.SetVec3(Shader::Format(Shader::SPOT_LIGHTS, Shader::AMBIENT, index), light.Ambient);
        _modelShader.SetVec3(Shader::Format(Shader::SPOT_LIGHTS, Shader::DIFFUSE, index), light.Diffuse);
        _modelShader.SetVec3(Shader::Format(Shader::SPOT_LIGHTS, Shader::SPECULAR, index), light.Specular);
        _modelShader.SetFloat(Shader::Format(Shader::SPOT_LIGHTS, Shader::CONSTANT, index), light.Linear);
        _modelShader.SetFloat(Shader::Format(Shader::SPOT_LIGHTS, Shader::LINEAR, index), light.Quadratic);
        _modelShader.SetFloat(Shader::Format(Shader::SPOT_LIGHTS, Shader::QUADRATIC, index), light.Quadratic);
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
