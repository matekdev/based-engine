#include "transform_component.hpp"

#include "scene.hpp"
#include "rigidbody_component.hpp"

#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/matrix_decompose.hpp>

TransformComponent::TransformComponent(const entt::entity &entity) : Component(entity), _constantBuffer(ConstantType::MODEL_MATRIX, ShaderStage::VERTEX_SHADER, TransformMatrixBuffer{})
{
}

glm::mat4 TransformComponent::GetTransform() const
{
    glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));
    return glm::translate(glm::mat4(1.0f), Position) * rotation * glm::scale(glm::mat4(1.0f), Scale);
}

physx::PxTransform TransformComponent::GetPhysicsTransform() const
{
    glm::vec3 scale;
    glm::quat rotation;
    glm::vec3 translation;
    glm::vec3 skew;
    glm::vec4 perspective;
    glm::decompose(GetTransform(), scale, rotation, translation, skew, perspective);
    return physx::PxTransform(
        physx::PxVec3(translation.x, translation.y, translation.z),
        physx::PxQuat(rotation.x, rotation.y, rotation.z, rotation.w));
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
