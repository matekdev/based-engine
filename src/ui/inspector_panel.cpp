#include "inspector_panel.hpp"

#include "scene.hpp"

#include "component/info_component.hpp"
#include "component/transform_component.hpp"

#include "imgui.h"
#include "imgui_stdlib.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <glm/gtc/type_ptr.hpp>
#include <limits>

#include "log.hpp"

InspectorPanel::InspectorPanel()
{
}

void InspectorPanel::Render()
{
    ImGui::Begin("Inspector");

    auto selectedEntity = Scene::ActiveScene->SelectedEntity;

    if (selectedEntity.has_value())
    {
        auto info = Scene::ActiveScene->Registry.try_get<InfoComponent>(selectedEntity.value());
        if (info && ImGui::CollapsingHeader("Info", ImGuiTreeNodeFlags_DefaultOpen))
        {
            ImGui::PushItemWidth(-1);
            ImGui::InputText("##label", &info->Name);
            ImGui::PopItemWidth();
        }

        auto transform = Scene::ActiveScene->Registry.try_get<TransformComponent>(selectedEntity.value());
        if (transform && ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
        {
            auto floatMin = std::numeric_limits<float>::min();
            auto floatMax = -std::numeric_limits<float>::max();

            ImGui::DragFloat3("Position", glm::value_ptr(transform->Position), 0.05f, floatMin, floatMax);
            ImGui::DragFloat3("Rotation", glm::value_ptr(transform->Rotation), 0.05f, floatMin, floatMax);
            ImGui::DragFloat3("Scale", glm::value_ptr(transform->Scale), 0.05f, floatMin, floatMax);
        }
    }

    ImGui::End();
}
