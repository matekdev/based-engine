#pragma once

#include "render/shader.hpp"

#include <entt/entt.hpp>

class LightComponent
{
public:
    glm::vec3 Ambient = glm::vec3(0.5f, 0.5f, 0.5f);
    glm::vec3 Diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
    glm::vec3 Specular = glm::vec3(0.5f, 0.5f, 0.5f);

    LightComponent(const entt::entity &entity);

private:
    entt::entity _entity;
};
