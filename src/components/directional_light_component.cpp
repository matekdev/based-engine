#include "directional_light_component.hpp"

#include "scene.hpp"
#include "transform_component.hpp"

DirectionalLightComponent::DirectionalLightComponent(const entt::entity &entity) : Component(entity),
                                                                                   _constantBuffer(ConstantType::DIRECTIONAL_LIGHT, ShaderStage::PIXEL_SHADER, DirectionalLightBuffer{})
{
}

void DirectionalLightComponent::Bind()
{
    auto transform = Scene::ActiveScene->Registry.get<TransformComponent>(_entity);

    _constantBuffer.Update(DirectionalLightBuffer{transform.GetDirection(), Ambient, Diffuse, Specular});
    _constantBuffer.Bind();
}
