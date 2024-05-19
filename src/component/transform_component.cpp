#include "component/transform_component.hpp"

#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

glm::mat4 TransformComponent::GetTransform()
{
    glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));
    return glm::translate(glm::mat4(1.0f), Position) * rotation * glm::scale(glm::mat4(1.0f), Scale);
}

glm::vec3 TransformComponent::GetDirection()
{
    return glm::normalize(glm::vec3(GetTransform()[2])); // z direction
}
