#pragma once

#include "component.hpp"
#include "render/constant_buffer.hpp"

#include <glm/glm.hpp>
#include <PxPhysicsAPI.h>

class TransformComponent : public Component
{
public:
    struct TransformMatrixBuffer
    {
        glm::mat4 Matrix;
    };

    TransformComponent(const entt::entity &entity);

    glm::mat4 GetTransform() const;
    glm::vec3 GetDirection() const;

    void SetPosition(const glm::vec3 &pos) const;
    glm::vec3 GetPosition() const;

    void SetRotation(const glm::vec3 &rot) const;
    glm::vec3 GetRotation() const;

    void SetScale(const glm::vec3 &scale);
    glm::vec3 GetScale() const;

    void SetBBox(const glm::vec3 &min, const glm::vec3 &max);

    void Bind();

private:
    ConstantBuffer<TransformMatrixBuffer> _constantBuffer;

    physx::PxRigidStatic *_pxRigidBody;
    physx::PxShape *_pxShape = nullptr;
    physx::PxMaterial *_pxMaterial;
    
    glm::vec3 _scale = glm::vec3(1.0f);
    physx::PxVec3 _extents;

    physx::PxBoxGeometry calculateBBox();
};
