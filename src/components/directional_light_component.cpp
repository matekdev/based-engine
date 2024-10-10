#include "directional_light_component.hpp"

#include "scene.hpp"
#include "transform_component.hpp"

DirectionalLightComponent::DirectionalLightComponent(const entt::entity &entity) : Component(entity), _constantBuffer(ConstantType::DIRECTIONAL_LIGHT, DirectionalLightBuffer{})
{
}

void DirectionalLightComponent::Bind()
{
    auto &transform = Scene::ActiveScene->Registry.get<TransformComponent>(_entity);
    _constantBuffer.Update(DirectionalLightBuffer{
        transform.GetDirection(),
        0.0f,
        Ambient,
        0.0f,
        Diffuse,
        0.0f,
        Specular,
        0.0f});
    _constantBuffer.Bind();
}
