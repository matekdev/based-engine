#include "scene_panel.hpp"

#include "log.hpp"
#include "scene.hpp"
#include "math/math.hpp"

#include "component/transform_component.hpp"
#include "component/info_component.hpp"
#include "component/model/model_component.hpp"
#include "imgui.h"
#include "imgui_stdlib.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>

ScenePanel::ScenePanel() : _pickingBuffer(FrameBuffer()),
                           _pickingShader(Shader("shaders/picking.vert", "shaders/picking.frag"))
{
}

void ScenePanel::Render(GLFWwindow *window)
{
    ImGui::ShowDemoWindow();

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("Scene");
    ImGui::PopStyleVar(3);

    auto scene = Scene::ActiveScene;
    auto panelSize = ImGui::GetContentRegionAvail();
    if (panelSize.x != _width || panelSize.y != _height)
    {
        scene->Resize(panelSize.x, panelSize.y);
        Resize(panelSize.x, panelSize.y);
    }

    uint64_t textureId = scene->GetRenderTextureId();
    ImGui::Image(reinterpret_cast<void *>(textureId), ImVec2{_width, _height}, ImVec2{0, 1}, ImVec2{1, 0});

    auto &camera = scene->GetCamera();
    camera.Input(panelSize.x, panelSize.y, window, ImGui::IsWindowHovered());
    camera.Update(panelSize.x, panelSize.y);

    HandleGizmo(window);

    ImGui::End();
}

void ScenePanel::Input(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        Scene::ActiveScene->SelectedEntity.reset();

    auto isUsingMouse = ImGuizmo::IsUsing() || Scene::ActiveScene->GetCamera().IsMouseLocked();

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS && !isUsingMouse)
        _activeGizmo = ImGuizmo::OPERATION::TRANSLATE;

    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS && !isUsingMouse)
        _activeGizmo = ImGuizmo::OPERATION::ROTATE;

    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS && !isUsingMouse)
        _activeGizmo = ImGuizmo::OPERATION::SCALE;

    if (glfwGetKey(window, GLFW_KEY_DELETE) == GLFW_PRESS && Scene::ActiveScene->SelectedEntity)
    {
        Scene::ActiveScene->Registry.destroy(Scene::ActiveScene->SelectedEntity.value());
        Scene::ActiveScene->SelectedEntity.reset();
    }
}

void ScenePanel::Resize(float width, float height)
{
    _width = width;
    _height = height;
    _pickingBuffer.CreateBuffer(width, height);
}

void ScenePanel::OnMouseClick()
{
    if (!Scene::ActiveScene->SelectedEntity || (!ImGuizmo::IsUsing() && !ImGuizmo::IsOver()))
        PickingPass();
}

void ScenePanel::HandleGizmo(GLFWwindow *window)
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

    auto shouldSnap = glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS;
    float snapValue = 0.5f; // Snap to 0.5m for translation/scale
    if (_activeGizmo == ImGuizmo::OPERATION::ROTATE)
        snapValue = 45.0f;

    float snapValues[3] = {snapValue, snapValue, snapValue};

    auto transformComponent = scene->Registry.try_get<TransformComponent>(scene->SelectedEntity.value());
    if (!transformComponent)
        return;

    auto &camera = scene->GetCamera();
    auto transform = transformComponent->GetTransform();
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

        glm::vec3 deltaRotation = rotation - transformComponent->Rotation;
        transformComponent->Position = position;
        transformComponent->Rotation += deltaRotation;
        transformComponent->Scale = scale;
    }
}

void ScenePanel::PickingPass()
{
    _pickingBuffer.Bind();

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    auto group = Scene::ActiveScene->Registry.view<ModelComponent, InfoComponent>();
    for (auto entity : group)
    {
        auto &info = group.get<InfoComponent>(entity);

        _pickingShader.Bind();
        _pickingShader.SetVec3(Shader::PICKING_COLOR, _pickingBuffer.EncodeId(info.Id));
        _pickingShader.SetMat4(Shader::CAMERA_MATRIX, Scene::ActiveScene->GetCamera().GetViewProjectionMatrix());
        _pickingShader.SetMat4(Shader::MODEL_MATRIX, Scene::ActiveScene->Registry.get<TransformComponent>(entity).GetTransform());

        auto &model = group.get<ModelComponent>(entity);
        model.Render(_pickingShader);
    }

    auto [mouseX, mouseY] = ImGui::GetMousePos();
    mouseX -= _viewPortBounds[0].x;
    mouseY -= _viewPortBounds[0].y;
    glm::vec2 viewportSize = _viewPortBounds[1] - _viewPortBounds[0];
    mouseY = viewportSize.y - mouseY;

    if (mouseX >= 0 && mouseY >= 0 && mouseX < (int)viewportSize.x && mouseY < (int)viewportSize.y)
    {
        auto id = _pickingBuffer.DecodePixel(mouseX, mouseY);
        auto it = std::find_if(std::begin(group), std::end(group), [&](const auto &entity)
                               {
                                auto &info = group.get<InfoComponent>(entity);
                                return info.Id == id; });
        if (it == std::end(group))
            Scene::ActiveScene->SelectedEntity.reset();
        else
            Scene::ActiveScene->SelectedEntity = *it;
    }

    _pickingBuffer.Unbind();
}
