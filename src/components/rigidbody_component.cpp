#include "rigidbody_component.hpp"

#include "components/transform_component.hpp"
#include "scene.hpp"

RigidBodyComponent::RigidBodyComponent(const entt::entity &entity) : Component(entity)
{
    auto &transform = Scene::ActiveScene->Registry.get<TransformComponent>(_entity);
    transform.EnablePhysics(true);
}

RigidBodyComponent::~RigidBodyComponent()
{
    auto &transform = Scene::ActiveScene->Registry.get<TransformComponent>(_entity);
    transform.EnablePhysics(false);
}
