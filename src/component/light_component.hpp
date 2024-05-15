#pragma once

#include "render/shader.hpp"

#include <entt/entt.hpp>

class LightComponent
{
public:
    LightComponent(const entt::entity &entity);

    void Render(Shader &shader);

private:
    int TODO;
};
