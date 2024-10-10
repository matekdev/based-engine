#include "transform_component.hpp"

#include "scene.hpp"

#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/matrix_decompose.hpp>

TransformComponent::TransformComponent(const entt::entity &entity) : Component(entity), _constantBuffer(ConstantType::MODEL_MATRIX, ModelMatrixBuffer{})
{
    _pxRigidActor = Scene::ActiveScene->GetPhysics()->createRigidStatic(physx::PxTransform(physx::PxIdentity));
    _pxMaterial = Scene::ActiveScene->GetPhysics()->createMaterial(0.5f, 0.5f, 0.5f);

    Scene::ActiveScene->GetPhysicsScene()->addActor(*_pxRigidActor);
}

TransformComponent::~TransformComponent()
{
    if (_pxRigidActor)
    {
        Scene::ActiveScene->GetPhysicsScene()->removeActor(*_pxRigidActor);
        _pxRigidActor->release();
    }

    if (_pxShape)
        _pxShape->release();

    if (_pxMaterial)
        _pxMaterial->release();
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
    auto pose = _pxRigidActor->getGlobalPose();
    pose.p = physx::PxVec3(pos.x, pos.y, pos.z);
    _pxRigidActor->setGlobalPose(pose);
}

glm::vec3 TransformComponent::GetPosition() const
{
    const auto pose = _pxRigidActor->getGlobalPose();
    return glm::vec3(pose.p.x, pose.p.y, pose.p.z);
}

void TransformComponent::SetRotation(const glm::vec3 &rot) const
{
    const auto pose = _pxRigidActor->getGlobalPose();
    const auto quat = glm::quat(rot);
    _pxRigidActor->setGlobalPose(physx::PxTransform(pose.p, physx::PxQuat(quat.x, quat.y, quat.z, quat.w)));
}

glm::vec3 TransformComponent::GetRotation() const
{
    const auto pose = _pxRigidActor->getGlobalPose();
    const auto rotation = glm::eulerAngles(glm::quat(pose.q.w, pose.q.x, pose.q.y, pose.q.z));
    return rotation;
}

void TransformComponent::SetScale(const glm::vec3 &scale)
{
    if (_scale == scale)
        return;

    _scale = scale;

    if (!_pxShape)
        return;

    _pxRigidActor->detachShape(*_pxShape);
    _pxShape->setGeometry(calculateBBox());
    _pxRigidActor->attachShape(*_pxShape);
}

glm::vec3 TransformComponent::GetScale() const
{
    return _scale;
}

void TransformComponent::SetBBox(const glm::vec3 &min, const glm::vec3 &max)
{
    if (_pxShape)
    {
        _pxRigidActor->detachShape(*_pxShape);
        PX_RELEASE(_pxShape);
    }

    auto minPoint = physx::PxVec3(min.x, min.y, min.z);
    auto maxPoint = physx::PxVec3(max.x, max.y, max.z);

    _extents = (maxPoint - minPoint) * 0.5f;
    _pxShape = Scene::ActiveScene->GetPhysics()->createShape(calculateBBox(), *_pxMaterial);
    _pxRigidActor->attachShape(*_pxShape);
}

bool TransformComponent::IsPicked(physx::PxRigidActor *shape) const
{
    return _pxRigidActor == shape;
}

void TransformComponent::EnablePhysics(const bool &enabled)
{
    auto transform = _pxRigidActor->getGlobalPose();
    Scene::ActiveScene->GetPhysicsScene()->removeActor(*_pxRigidActor);
    _pxRigidActor->release();

    if (enabled)
        _pxRigidActor = Scene::ActiveScene->GetPhysics()->createRigidDynamic(transform);
    else
        _pxRigidActor = Scene::ActiveScene->GetPhysics()->createRigidStatic(transform);

    _pxRigidActor->attachShape(*_pxShape);
    Scene::ActiveScene->GetPhysicsScene()->addActor(*_pxRigidActor);
}

void TransformComponent::ResetVelocity()
{
    if (_pxRigidActor->getConcreteType() != physx::PxConcreteType::eRIGID_DYNAMIC)
        return;

    auto *rigidBody = static_cast<physx::PxRigidDynamic *>(_pxRigidActor);
    if (!rigidBody)
        return;

    rigidBody->setLinearVelocity(physx::PxVec3(0.0f));
    rigidBody->setAngularVelocity(physx::PxVec3(0.0f));
}

void TransformComponent::Bind()
{
    _constantBuffer.Update(ModelMatrixBuffer{GetTransform()});
    _constantBuffer.Bind();
}

physx::PxBoxGeometry TransformComponent::calculateBBox()
{
    return physx::PxBoxGeometry(
        std::max(std::abs(_extents.x * _scale.x), 0.01f),
        std::max(std::abs(_extents.y * _scale.y), 0.01f),
        std::max(std::abs(_extents.z * _scale.z), 0.01f));
}
