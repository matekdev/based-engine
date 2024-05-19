#include "scene.hpp"

#include "component/info_component.hpp"
#include "component/transform_component.hpp"
#include "component/directional_light_component.hpp"
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
    for (auto it = directionalLightGroup.begin(); it != directionalLightGroup.end(); ++it)
    {
        int index = std::distance(directionalLightGroup.begin(), it);
        auto &light = directionalLightGroup.get<DirectionalLightComponent>(*it);
        auto &transform = directionalLightGroup.get<TransformComponent>(*it);

        _modelShader.Bind();
        _modelShader.SetInt(Shader::DIRECTIONAL_LIGHT_COUNT, directionalLightGroup.size_hint());
        _modelShader.SetVec3(Shader::Format(Shader::DIRECTIONAL_LIGHTS, Shader::DIRECTION, index), transform.GetDirection());
        _modelShader.SetVec3(Shader::Format(Shader::DIRECTIONAL_LIGHTS, Shader::AMBIENT, index), light.Ambient);
        _modelShader.SetVec3(Shader::Format(Shader::DIRECTIONAL_LIGHTS, Shader::DIFFUSE, index), light.Diffuse);
        _modelShader.SetVec3(Shader::Format(Shader::DIRECTIONAL_LIGHTS, Shader::SPECULAR, index), light.Specular);
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
