#pragma once

#include "component.hpp"

#include <glm/glm.hpp>
#include <entt/entt.hpp>

class DirectionalLightComponent : public Component
{
public:
    glm::vec3 Ambient = glm::vec3(0.5f, 0.5f, 0.5f);
    glm::vec3 Diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
    glm::vec3 Specular = glm::vec3(0.5f, 0.5f, 0.5f);

    DirectionalLightComponent(const entt::entity &entity);

private:
    entt::entity _entity;
};
