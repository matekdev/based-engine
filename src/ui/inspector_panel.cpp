#include "inspector_panel.hpp"

#include "scene.hpp"
#include "icons/regular.hpp"
#include "icons/brands.hpp"

#include "components/info_component.hpp"
#include "components/transform_component.hpp"

#include <imgui.h>
#include <imgui_stdlib.h>
#include <glm/gtc/type_ptr.hpp>

void InspectorPanel::Draw()
{
    ImGui::Begin("Inspector");

    auto selectedEntity = Scene::ActiveScene->SelectedEntity;
    if (!selectedEntity.has_value())
    {
        ImGui::End();
        return;
    }

    if (ImGui::CollapsingHeader(ICON_FA_UNITY " Entity", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::BeginGroup();

        auto info = Scene::ActiveScene->Registry.try_get<InfoComponent>(selectedEntity.value());
        ImGui::PushItemWidth(-1);
        ImGui::InputText("##label", &info->Name);
        ImGui::PopItemWidth();

        auto transform = Scene::ActiveScene->Registry.try_get<TransformComponent>(selectedEntity.value());
        auto floatMin = std::numeric_limits<float>::min();
        auto floatMax = -std::numeric_limits<float>::max();
        ImGui::DragFloat3(ICON_FA_ARROW_UP_RIGHT_FROM_SQUARE " Position", glm::value_ptr(transform->Position), 0.05f, floatMin, floatMax);
        ImGui::DragFloat3(ICON_FA_ARROWS_ROTATE " Rotation", glm::value_ptr(transform->Rotation), 0.05f, floatMin, floatMax);
        ImGui::DragFloat3(ICON_FA_MAGNIFYING_GLASS_PLUS " Scale", glm::value_ptr(transform->Scale), 0.05f, floatMin, floatMax);

        ImGui::EndGroup();
    }

    ImGui::End();
}
