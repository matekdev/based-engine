#include "entities_panel.hpp"

#include "scene.hpp"
#include "components/info_component.hpp"

#include "imgui.h"
#include "icons/regular.hpp"

void EntitiesPanel::Draw()
{
    ImGui::Begin("Entities");

    if (ImGui::Button(ICON_FA_CIRCLE_PLUS " Add Entity", ImVec2(-1, 0)))
        Scene::ActiveScene->CreateNewEntity();

    ImGui::Separator();
    ImGui::Spacing();

    auto group = Scene::ActiveScene->Registry.view<InfoComponent>();
    for (auto entity : group)
    {
        auto info = group.get<InfoComponent>(entity);
        ImGui::PushID(info.Id);

        auto name = std::string(ICON_FA_CUBE) + " " + info.Name;
        if (ImGui::Selectable(name.c_str(), Scene::ActiveScene->SelectedEntity.has_value() && Scene::ActiveScene->SelectedEntity.value() == entity))
        {
            Scene::ActiveScene->SelectedEntity = entity;
        }

        ImGui::PopID();
    }

    ImGui::End();
}
