#include "scene_panel.hpp"

#include "scene.hpp"
#include "components/transform_component.hpp"
#include "common/glfw_util.hpp"
#include "common/math.hpp"
#include "log.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>
#include <PxPhysicsAPI.h>

bool ScenePanel::IsHovered()
{
    return _isHovered;
}

float ScenePanel::GetWidth()
{
    return _width;
}

float ScenePanel::GetHeight()
{
    return _height;
}

void ScenePanel::Draw()
{
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("Scene");
    ImGui::PopStyleVar(3);

    _isHovered = ImGui::IsWindowHovered();

    auto panelSize = ImGui::GetContentRegionAvail();
    if (panelSize.x != _width || panelSize.y != _height)
    {
        _width = panelSize.x;
        _height = panelSize.y;

        Scene::ActiveScene->OnResize();
    }

    ImGui::Image(Scene::ActiveScene->GetShaderResourceView(), panelSize);

    UpdateInput();
    UpdateGizmo();

    ImGui::End();
}

void ScenePanel::UpdateInput()
{
    UpdatePicking();

    auto window = GLFWUtil::GetNativeWindow();
    if (GLFWUtil::IsButtonPressed(GLFW_KEY_ESCAPE))
        Scene::ActiveScene->SelectedEntity.reset();

    auto isUsingMouse = ImGuizmo::IsUsing() || GLFWUtil::IsMouseLocked();
    auto &io = ImGui::GetIO();

    if (!isUsingMouse && !io.WantTextInput)
    {
        if (GLFWUtil::IsButtonPressed(GLFW_KEY_W))
            _activeGizmo = ImGuizmo::OPERATION::TRANSLATE;

        if (GLFWUtil::IsButtonPressed(GLFW_KEY_E))
            _activeGizmo = ImGuizmo::OPERATION::ROTATE;

        if (GLFWUtil::IsButtonPressed(GLFW_KEY_R))
            _activeGizmo = ImGuizmo::OPERATION::SCALE;
    }

    if (GLFWUtil::IsButtonPressed(GLFW_KEY_DELETE) && Scene::ActiveScene->SelectedEntity.has_value())
    {
        Scene::ActiveScene->Registry.destroy(Scene::ActiveScene->SelectedEntity.value());
        Scene::ActiveScene->SelectedEntity.reset();
    }
}

void ScenePanel::UpdateGizmo()
{
    auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
    auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
    auto viewportOffset = ImGui::GetWindowPos();
    _viewPortBounds[0] = {viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y};
    _viewPortBounds[1] = {viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y};

    auto scene = Scene::ActiveScene;
    if (!scene->SelectedEntity.has_value())
        return;

    ImGuizmo::SetOrthographic(false);
    ImGuizmo::SetDrawlist();
    ImGuizmo::SetRect(_viewPortBounds[0].x, _viewPortBounds[0].y, _viewPortBounds[1].x - _viewPortBounds[0].x, _viewPortBounds[1].y - _viewPortBounds[0].y);

    auto shouldSnap = glfwGetKey(GLFWUtil::GetNativeWindow(), GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS;
    float snapValue = 0.5f; // Snap to 0.5m for translation/scale
    if (_activeGizmo == ImGuizmo::OPERATION::ROTATE)
        snapValue = 45.0f;

    float snapValues[3] = {snapValue, snapValue, snapValue};

    auto transformComponent = scene->Registry.try_get<TransformComponent>(scene->SelectedEntity.value());
    if (!transformComponent)
        return;

    auto &camera = scene->GetActiveCamera();
    glm::mat4 transform = transformComponent->GetTransform();
    ImGuizmo::Manipulate(glm::value_ptr(camera.GetViewMatrix()),
                         glm::value_ptr(camera.GetProjectionMatrix()),
                         _activeGizmo, ImGuizmo::WORLD,
                         glm::value_ptr(transform),
                         nullptr,
                         shouldSnap ? snapValues : nullptr,
                         nullptr,
                         nullptr);

    if (ImGuizmo::IsUsing())
    {
        glm::vec3 position, rotation, scale;
        DecomposeTransform(transform, position, rotation, scale);

        glm::vec3 currentRotation = transformComponent->GetRotation();
        glm::vec3 newRotation = currentRotation + (rotation - currentRotation);

        transformComponent->SetPosition(position);
        transformComponent->SetRotation(newRotation);
        transformComponent->SetScale(scale);
    }
}

void ScenePanel::UpdatePicking()
{
    if (!GLFWUtil::IsMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT) || GLFWUtil::IsMouseLocked())
        return;

    if (Scene::ActiveScene->SelectedEntity && (ImGuizmo::IsUsing() || ImGuizmo::IsOver()))
        return;

    auto [mouseX, mouseY] = ImGui::GetMousePos();
    mouseX -= _viewPortBounds[0].x;
    mouseY -= _viewPortBounds[0].y;

    auto viewportSize = _viewPortBounds[1] - _viewPortBounds[0];
    mouseY = viewportSize.y - mouseY;

    // Make sure we are within bounds of the window.
    if (mouseX >= viewportSize.x || mouseX <= 0 || mouseY >= viewportSize.y || mouseY <= 0)
        return;

    // Thanks https://www.opengl-tutorial.org/miscellaneous/clicking-on-objects/picking-with-a-physics-library/
    glm::vec4 startNDC(((float)mouseX / (float)_width - 0.5f) * 2.0f, ((float)mouseY / (float)_height - 0.5f) * 2.0f, -1.0, 1.0f);
    glm::vec4 endNDC(((float)mouseX / (float)_width - 0.5f) * 2.0f, ((float)mouseY / (float)_height - 0.5f) * 2.0f, 0.0, 1.0f);

    auto &camera = Scene::ActiveScene->GetActiveCamera();
    auto inverseProjectionMatrix = glm::inverse(camera.GetProjectionMatrix());
    auto inverseViewMatrix = glm::inverse(camera.GetViewMatrix());

    auto cameraStart = inverseProjectionMatrix * startNDC;
    cameraStart /= cameraStart.w;

    auto worldStart = inverseViewMatrix * cameraStart;
    worldStart /= worldStart.w;

    auto cameraEnd = inverseProjectionMatrix * endNDC;
    cameraEnd /= cameraEnd.w;

    auto worldEnd = inverseViewMatrix * cameraEnd;
    worldEnd /= worldEnd.w;

    auto worldDirection(worldEnd - worldStart);
    worldDirection = glm::normalize(worldDirection);

    auto origin = glm::vec3(worldStart);
    auto dir = glm::normalize(worldDirection);

    physx::PxRaycastBuffer result;
    auto hit = Scene::ActiveScene->GetPhysicsScene()->raycast(physx::PxVec3(origin.x, origin.y, origin.z), physx::PxVec3(dir.x, dir.y, dir.z), 1000.f, result);
    if (!hit)
    {
        Scene::ActiveScene->SelectedEntity.reset();
        return;
    }

    const auto modelGroup = Scene::ActiveScene->Registry.view<TransformComponent>();
    for (const auto &entity : modelGroup)
    {
        auto &transform = modelGroup.get<TransformComponent>(entity);
        if (transform.IsPicked(result.block.actor))
        {
            Scene::ActiveScene->SelectedEntity = entity;
            break;
        }
    }
}
