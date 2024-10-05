#pragma once

#include "component.hpp"

#include <PxPhysicsAPI.h>
#include <glm/glm.hpp>

class RigidBodyComponent : public Component
{
public:
    RigidBodyComponent(const entt::entity &entity);
    ~RigidBodyComponent();

    glm::vec3 GetTransform() const;

private:
    physx::PxRigidDynamic *_pxRigidBody;
    physx::PxMaterial *_pxMaterial;
};
