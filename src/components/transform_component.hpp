#pragma once

#include "component.hpp"
#include "render/constant_buffer.hpp"

#include <glm/glm.hpp>
#include <PxPhysicsAPI.h>

class TransformComponent : public Component
{
public:
    struct ModelMatrixBuffer
    {
        glm::mat4 Matrix;
    };

    TransformComponent(const entt::entity &entity);
    ~TransformComponent();

    glm::mat4 GetTransform() const;
    glm::vec3 GetDirection() const;

    void SetPosition(const glm::vec3 &pos) const;
    glm::vec3 GetPosition() const;

    void SetRotation(const glm::vec3 &rot) const;
    glm::vec3 GetRotation() const;

    void SetScale(const glm::vec3 &scale);
    glm::vec3 GetScale() const;

    void SetBBox(const glm::vec3 &min, const glm::vec3 &max);

    bool IsPicked(physx::PxRigidActor *shape) const;
    void EnablePhysics(const bool &enable);
    void ResetVelocity();

    void Bind();

private:
    ConstantBuffer<ModelMatrixBuffer> _constantBuffer;

    // This component has physics... in an ideal world this should be a seperate component.
    physx::PxRigidActor *_pxRigidActor = nullptr;
    physx::PxShape *_pxShape = nullptr;
    physx::PxMaterial *_pxMaterial = nullptr;

    glm::vec3 _scale = glm::vec3(1.0f);
    physx::PxVec3 _extents;

    physx::PxBoxGeometry calculateBBox();
};
