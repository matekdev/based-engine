#pragma once

#include <glm/glm.hpp>
#include <entt/entt.hpp>

class PointLightComponent
{
public:
    glm::vec3 Ambient = glm::vec3(0.5f, 0.5f, 0.5f);
    glm::vec3 Diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
    glm::vec3 Specular = glm::vec3(0.5f, 0.5f, 0.5f);

    // Attenuation values
    // https://wiki.ogre3d.org/tiki-index.php?page=-Point+Light+Attenuation
    float Constant = 1.0f;
    float Linear = 0.7f;
    float Quadratic = 1.8f;

    PointLightComponent(const entt::entity &entity);

private:
    entt::entity _entity;
};
