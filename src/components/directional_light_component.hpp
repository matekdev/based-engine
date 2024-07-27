#pragma once

#include "component.hpp"
#include "render/constant_buffer.hpp"

#include <glm/glm.hpp>
#include <entt/entt.hpp>

class DirectionalLightComponent : public Component
{
public:
    struct alignas(16) DirectionalLightBuffer
    {
        glm::vec3 Direction;
        glm::vec3 Ambient;
        glm::vec3 Diffuse;
        glm::vec3 Specular;
    };

    glm::vec3 Ambient = glm::vec3(0.5f, 0.5f, 0.5f);
    glm::vec3 Diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
    glm::vec3 Specular = glm::vec3(0.5f, 0.5f, 0.5f);

    DirectionalLightComponent(const entt::entity &entity);

    void Bind();

private:
    ConstantBuffer<DirectionalLightBuffer> _constantBuffer;
};
