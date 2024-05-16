#pragma once

#include "render/shader.hpp"

#include <entt/entt.hpp>

class LightComponent
{
public:
    glm::vec3 Color = glm::vec3(1.0f);

    LightComponent(const entt::entity &entity);

    void Render(Shader &shader);

private:
    entt::entity _entity;
};
