#pragma once

#include "component.hpp"

#include <PxPhysicsAPI.h>

class RigidBodyComponent : public Component
{
public:
    RigidBodyComponent(const entt::entity &entity);
    ~RigidBodyComponent();

private:
    physx::PxRigidDynamic *_pxRigidBody;
    physx::PxMaterial* _pxMaterial;
};
