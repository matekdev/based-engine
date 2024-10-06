#include "rigidbody_component.hpp"

#include "components/transform_component.hpp"
#include "scene.hpp"

RigidBodyComponent::RigidBodyComponent(const entt::entity &entity) : Component(entity)
{
}

RigidBodyComponent::~RigidBodyComponent()
{
}
