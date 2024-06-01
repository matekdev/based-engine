#include "scene.hpp"

#include "component/info_component.hpp"
#include "component/transform_component.hpp"
#include "component/directional_light_component.hpp"
#include "component/point_light_component.hpp"
#include "component/spot_light_component.hpp"
#include "component/model/model_component.hpp"

Scene::Scene() : _camera(Camera()),
                 _frameBuffer(FrameBuffer()),
                 _shadowMap(ShadowMap()),
                 _skybox(SkyBox()),
                 _modelShader(Shader("shaders/model.vert", "shaders/model.frag")),
                 _lightShader(Shader("shaders/model.vert", "shaders/model.frag")),
                 _outlineShader(Shader("shaders/model.vert", "shaders/outline.frag")),
                 _skyboxShader(Shader("shaders/skybox.vert", "shaders/skybox.frag")),
                 _shadowMapShader(Shader("shaders/shadow_map.vert", "shaders/shadow_map.frag"))
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
    _shadowMap.CreateBuffer(width, height);
}

void Scene::Render()
{
    CalculateDeltaTime();

    ShadowRenderPass();
    NormalRenderPass();
}

void Scene::ShadowRenderPass()
{
    _shadowMap.Bind();

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    auto directionalLightGroup = Scene::ActiveScene->Registry.view<DirectionalLightComponent, TransformComponent>();
    for (auto it = directionalLightGroup.begin(); it != directionalLightGroup.end(); ++it)
    {
        int index = std::distance(directionalLightGroup.begin(), it);
        auto &light = directionalLightGroup.get<DirectionalLightComponent>(*it);
        auto &transform = directionalLightGroup.get<TransformComponent>(*it);

        _shadowMapShader.Bind();

        glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.1f, 10.0f);
        glm::mat4 lightView = glm::lookAt(transform.Position, transform.Position + transform.GetDirection(), glm::vec3(0.0, 1.0, 0.0));
        _lightSpaceMatrix = lightProjection * lightView;
        _shadowMapShader.SetMat4(Shader::LIGHT_SPACE_MATRIX, _lightSpaceMatrix);
        RenderModels(_shadowMapShader);
    }

    _shadowMap.Unbind();
}

void Scene::NormalRenderPass()
{
    _frameBuffer.Bind();

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    RenderModels(_modelShader);
    RenderLights(_modelShader);
    RenderSkyBox(_modelShader);

    _frameBuffer.Unbind();
}

void Scene::RenderModels(Shader &shader)
{
    auto modelGroup = Scene::ActiveScene->Registry.view<ModelComponent, TransformComponent>();
    for (auto entity : modelGroup)
    {
        auto &model = modelGroup.get<ModelComponent>(entity);
        auto &transform = modelGroup.get<TransformComponent>(entity);

        shader.Bind();
        shader.SetVec3(Shader::MATERIAL_AMBIENT, model.Ambient);
        shader.SetVec3(Shader::MATERIAL_DIFFUSE, model.Diffuse);
        shader.SetVec3(Shader::MATERIAL_SPECULAR, model.Specular);
        shader.SetFloat(Shader::MATERIAL_SHININESS, model.Shininess);

        shader.SetVec3(Shader::CAMERA_POSITION, _camera.GetPosition());
        shader.SetMat4(Shader::CAMERA_MATRIX, _camera.GetViewProjectionMatrix());
        shader.SetMat4(Shader::MODEL_MATRIX, transform.GetTransform());
        shader.SetMat4(Shader::LIGHT_SPACE_MATRIX, _lightSpaceMatrix);
        shader.SetBool(Shader::HAS_TEXTURES, model.HasTextures());

        _shadowMap.BindTexture(shader);
        _skybox.BindTexture(shader);

        model.Render(shader);
    }
}

void Scene::RenderLights(Shader &shader)
{
    auto directionalLightGroup = Scene::ActiveScene->Registry.view<DirectionalLightComponent, TransformComponent>();
    shader.Bind();
    shader.SetInt(Shader::DIRECTIONAL_LIGHT_COUNT, directionalLightGroup.size_hint());

    for (auto it = directionalLightGroup.begin(); it != directionalLightGroup.end(); ++it)
    {
        int index = std::distance(directionalLightGroup.begin(), it);
        auto &light = directionalLightGroup.get<DirectionalLightComponent>(*it);
        auto &transform = directionalLightGroup.get<TransformComponent>(*it);

        shader.SetVec3(Shader::Format(Shader::DIRECTIONAL_LIGHTS, Shader::DIRECTION, index), transform.GetDirection());
        shader.SetVec3(Shader::Format(Shader::DIRECTIONAL_LIGHTS, Shader::AMBIENT, index), light.Ambient);
        shader.SetVec3(Shader::Format(Shader::DIRECTIONAL_LIGHTS, Shader::DIFFUSE, index), light.Diffuse);
        shader.SetVec3(Shader::Format(Shader::DIRECTIONAL_LIGHTS, Shader::SPECULAR, index), light.Specular);
    }

    auto pointLightGroup = Scene::ActiveScene->Registry.view<PointLightComponent, TransformComponent>();
    shader.Bind();
    shader.SetInt(Shader::POINT_LIGHT_COUNT, pointLightGroup.size_hint());

    for (auto it = pointLightGroup.begin(); it != pointLightGroup.end(); ++it)
    {
        int index = std::distance(pointLightGroup.begin(), it);
        auto &light = pointLightGroup.get<PointLightComponent>(*it);
        auto &transform = pointLightGroup.get<TransformComponent>(*it);

        shader.SetVec3(Shader::Format(Shader::POINT_LIGHTS, Shader::POSITION, index), transform.Position);
        shader.SetVec3(Shader::Format(Shader::POINT_LIGHTS, Shader::AMBIENT, index), light.Ambient);
        shader.SetVec3(Shader::Format(Shader::POINT_LIGHTS, Shader::DIFFUSE, index), light.Diffuse);
        shader.SetVec3(Shader::Format(Shader::POINT_LIGHTS, Shader::SPECULAR, index), light.Specular);
        shader.SetFloat(Shader::Format(Shader::POINT_LIGHTS, Shader::CONSTANT, index), light.Linear);
        shader.SetFloat(Shader::Format(Shader::POINT_LIGHTS, Shader::LINEAR, index), light.Quadratic);
        shader.SetFloat(Shader::Format(Shader::POINT_LIGHTS, Shader::QUADRATIC, index), light.Quadratic);
    }

    auto spotLightGroup = Scene::ActiveScene->Registry.view<SpotLightComponent, TransformComponent>();
    shader.Bind();
    shader.SetInt(Shader::SPOT_LIGHT_COUNT, spotLightGroup.size_hint());

    for (auto it = spotLightGroup.begin(); it != spotLightGroup.end(); ++it)
    {
        int index = std::distance(spotLightGroup.begin(), it);
        auto &light = spotLightGroup.get<SpotLightComponent>(*it);
        auto &transform = spotLightGroup.get<TransformComponent>(*it);

        shader.SetVec3(Shader::Format(Shader::SPOT_LIGHTS, Shader::POSITION, index), transform.Position);
        shader.SetVec3(Shader::Format(Shader::SPOT_LIGHTS, Shader::DIRECTION, index), transform.GetDirection());
        shader.SetFloat(Shader::Format(Shader::SPOT_LIGHTS, Shader::RADIUS, index), glm::cos(glm::radians(light.Radius)));
        shader.SetFloat(Shader::Format(Shader::SPOT_LIGHTS, Shader::RADIUS, index), glm::cos(glm::radians(light.Radius + 5)));
        shader.SetVec3(Shader::Format(Shader::SPOT_LIGHTS, Shader::AMBIENT, index), light.Ambient);
        shader.SetVec3(Shader::Format(Shader::SPOT_LIGHTS, Shader::DIFFUSE, index), light.Diffuse);
        shader.SetVec3(Shader::Format(Shader::SPOT_LIGHTS, Shader::SPECULAR, index), light.Specular);
        shader.SetFloat(Shader::Format(Shader::SPOT_LIGHTS, Shader::CONSTANT, index), light.Linear);
        shader.SetFloat(Shader::Format(Shader::SPOT_LIGHTS, Shader::LINEAR, index), light.Quadratic);
        shader.SetFloat(Shader::Format(Shader::SPOT_LIGHTS, Shader::QUADRATIC, index), light.Quadratic);
    }
}

void Scene::RenderSkyBox(Shader &shader)
{
    glDepthFunc(GL_LEQUAL);

    _skyboxShader.Bind();
    _skyboxShader.SetMat4(Shader::CAMERA_VIEW_MATRIX, glm::mat4(glm::mat3(_camera.GetViewMatrix())));
    _skyboxShader.SetMat4(Shader::CAMERA_PROJECTION, _camera.GetProjectionMatrix());
    _skybox.Render(_skyboxShader);

    glDepthFunc(GL_LESS);
}

// unused for now
void Scene::RenderWithOutline(ModelComponent &model, TransformComponent &transform)
{
    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glStencilMask(0xFF);

    model.Render(_modelShader);

    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilMask(0x00);
    glDisable(GL_DEPTH_TEST);

    auto scaledTransform = transform;
    scaledTransform.Scale *= 1.02;

    _outlineShader.Bind();
    _outlineShader.SetMat4(Shader::CAMERA_MATRIX, _camera.GetViewProjectionMatrix());
    _outlineShader.SetMat4(Shader::MODEL_MATRIX, scaledTransform.GetTransform());

    model.Render(_outlineShader);

    glStencilMask(0xFF);
    glStencilFunc(GL_ALWAYS, 0, 0xFF);
    glEnable(GL_DEPTH_TEST);
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
