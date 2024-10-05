#include "rigidbody_component.hpp"

#include "components/transform_component.hpp"
#include "scene.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

RigidBodyComponent::RigidBodyComponent(const entt::entity &entity) : Component(entity)
{
    TransformComponent transform = Scene::ActiveScene->Registry.get<TransformComponent>(entity);
    _pxRigidBody = Scene::ActiveScene->GetPhysics()->createRigidDynamic(transform.GetPhysicsTransform());
    _pxMaterial = Scene::ActiveScene->GetPhysics()->createMaterial(0.5f, 0.5f, 0.6f);

    auto *shape = Scene::ActiveScene->GetPhysics()->createShape(physx::PxBoxGeometry(2.0f, 2.0f, 2.0f), *_pxMaterial);
    _pxRigidBody->attachShape(*shape);

    Scene::ActiveScene->GetPhysicsScene()->addActor(*_pxRigidBody);
}

RigidBodyComponent::~RigidBodyComponent()
{
    // TODO: Remove from scene, cleanup??
}

glm::vec3 RigidBodyComponent::GetTransform() const
{
    const auto transform = _pxRigidBody->getGlobalPose();
    const auto translation = glm::vec3(transform.p.x, transform.p.y, transform.p.z);
    return translation;
}
