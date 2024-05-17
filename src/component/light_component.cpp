#include "light_component.hpp"

#include "scene.hpp"
#include "component/transform_component.hpp"

LightComponent::LightComponent(const entt::entity &entity) : _entity(entity)
{
}
