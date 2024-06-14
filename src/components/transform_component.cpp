#include "transform_component.hpp"

#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

TransformComponent::TransformComponent() : _constantBuffer(ConstantType::MODEL_MATRIX, ShaderStage::VERTEX_SHADER, TransformMatrixBuffer{})
{
}

glm::mat4 TransformComponent::GetTransform() const
{
    glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));
    return glm::translate(glm::mat4(1.0f), Position) * rotation * glm::scale(glm::mat4(1.0f), Scale);
}

glm::vec3 TransformComponent::GetDirection() const
{
    return glm::normalize(glm::vec3(GetTransform()[2])); // z direction
}

void TransformComponent::Bind()
{
    _constantBuffer.Update(TransformMatrixBuffer{GetTransform()});
    _constantBuffer.Bind();
}