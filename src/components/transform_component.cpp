#include "transform_component.hpp"

#include "scene.hpp"

#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/matrix_decompose.hpp>

TransformComponent::TransformComponent(const entt::entity &entity) : Component(entity), _constantBuffer(ConstantType::MODEL_MATRIX, ShaderStage::VERTEX_SHADER, TransformMatrixBuffer{})
{
    _pxRigidBody = Scene::ActiveScene->GetPhysics()->createRigidStatic(physx::PxTransform(physx::PxIdentity));

    auto material = Scene::ActiveScene->GetPhysics()->createMaterial(0.5f, 0.5f, 0.5f);
    auto *shape = Scene::ActiveScene->GetPhysics()->createShape(physx::PxBoxGeometry(0.5f, 0.5f, 0.5f), *material);
    _pxRigidBody->attachShape(*shape);

    Scene::ActiveScene->GetPhysicsScene()->addActor(*_pxRigidBody);
}

glm::mat4 TransformComponent::GetTransform() const
{
    auto rotation = glm::toMat4(glm::quat(GetRotation()));
    return glm::translate(glm::mat4(1.0f), GetPosition()) * rotation * glm::scale(glm::mat4(1.0f), GetScale());
}

glm::vec3 TransformComponent::GetDirection() const
{
    return glm::normalize(glm::vec3(GetTransform()[2])); // z direction
}

void TransformComponent::SetPosition(const glm::vec3 &pos) const
{
    auto pose = _pxRigidBody->getGlobalPose();
    pose.p = physx::PxVec3(pos.x, pos.y, pos.z);
    _pxRigidBody->setGlobalPose(pose);
}

glm::vec3 TransformComponent::GetPosition() const
{
    const auto pose = _pxRigidBody->getGlobalPose();
    return glm::vec3(pose.p.x, pose.p.y, pose.p.z);
}

void TransformComponent::SetRotation(const glm::vec3 &rot) const
{
    const auto pose = _pxRigidBody->getGlobalPose();
    const auto quat = glm::quat(rot);
    _pxRigidBody->setGlobalPose(physx::PxTransform(pose.p, physx::PxQuat(quat.x, quat.y, quat.z, quat.w)));
}

glm::vec3 TransformComponent::GetRotation() const
{
    const auto pose = _pxRigidBody->getGlobalPose();
    const auto rotation = glm::eulerAngles(glm::quat(pose.q.w, pose.q.x, pose.q.y, pose.q.z));
    return rotation;
}

void TransformComponent::SetScale(const glm::vec3 &scale)
{
    _scale = scale;
}

glm::vec3 TransformComponent::GetScale() const
{
    return _scale;
}

void TransformComponent::Bind()
{
    _constantBuffer.Update(TransformMatrixBuffer{GetTransform()});
    _constantBuffer.Bind();
}
