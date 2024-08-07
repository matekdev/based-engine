#pragma once

#include "component.hpp"
#include "render/constant_buffer.hpp"

#include <glm/glm.hpp>

class TransformComponent : public Component
{
public:
    struct alignas(16) TransformMatrixBuffer
    {
        glm::mat4 Matrix;
    };

    TransformComponent(const entt::entity &entity);

    glm::vec3 Position = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 Rotation = glm::vec3(0.0f);
    glm::vec3 Scale = glm::vec3(1.0f);

    glm::mat4 GetTransform() const;
    glm::vec3 GetDirection() const;

    void Bind();

private:
    ConstantBuffer<TransformMatrixBuffer> _constantBuffer;
};
