#include "inspector_panel.hpp"

#include "scene.hpp"

#include "component/info_component.hpp"
#include "component/transform_component.hpp"
#include "component/model/model_component.hpp"
#include "ui/icon.hpp"
#include "ui/icon_brands.hpp"

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
        if (ImGui::CollapsingHeader(ICON_FA_UNITY " Entity", ImGuiTreeNodeFlags_DefaultOpen))
        {
            auto info = Scene::ActiveScene->Registry.try_get<InfoComponent>(selectedEntity.value());
            ImGui::PushItemWidth(-1);
            ImGui::InputText("##label", &info->Name);
            ImGui::PopItemWidth();

            auto transform = Scene::ActiveScene->Registry.try_get<TransformComponent>(selectedEntity.value());
            auto floatMin = std::numeric_limits<float>::min();
            auto floatMax = -std::numeric_limits<float>::max();
            ImGui::DragFloat3("Position", glm::value_ptr(transform->Position), 0.05f, floatMin, floatMax);
            ImGui::DragFloat3("Rotation", glm::value_ptr(transform->Rotation), 0.05f, floatMin, floatMax);
            ImGui::DragFloat3("Scale", glm::value_ptr(transform->Scale), 0.05f, floatMin, floatMax);
        }

        ComponentHeader<ModelComponent>(ICON_FA_PERSON " Model",
                                        []() {});

        ImGui::Spacing();
        ImGui::Spacing();
        ImGui::Spacing();

        if (ImGui::Button(ICON_FA_CIRCLE_PLUS " Add Component", ImVec2(-1, 0)))
            ImGui::OpenPopup("AddComponent");

        if (ImGui::BeginPopup("AddComponent"))
        {
            AddComponentEntry<ModelComponent>(ICON_FA_PERSON " Model");
            ImGui::EndPopup();
        }
    }

    ImGui::End();
}

template <typename T>
void InspectorPanel::ComponentHeader(const std::string &name, const std::function<void()> &options)
{
    auto flags = ImGuiTreeNodeFlags_AllowOverlap | ImGuiTreeNodeFlags_DefaultOpen;
    auto selectedEntity = Scene::ActiveScene->SelectedEntity;

    auto component = Scene::ActiveScene->Registry.try_get<T>(selectedEntity.value());
    if (component)
    {
        auto isOpen = ImGui::CollapsingHeader(name.c_str(), flags);
        ImGui::SameLine();
        ImGui::SetCursorPosX(ImGui::GetWindowWidth() - ImGui::GetStyle().ItemSpacing.x - ImGui::GetFrameHeight());
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 18.0f);
        if (ImGui::Button(ICON_FA_XMARK))
            Scene::ActiveScene->Registry.remove<T>(Scene::ActiveScene->SelectedEntity.value());
        ImGui::PopStyleVar();

        if (isOpen)
            options();
    }
}

template <typename T>
void InspectorPanel::RemoveComponentButton()
{
    ImGui::SameLine();
    ImGui::SetCursorPosX(ImGui::GetWindowWidth() - ImGui::GetStyle().ItemSpacing.x - ImGui::GetFrameHeight());
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 18.0f);
    if (ImGui::Button(ICON_FA_XMARK))
        Scene::ActiveScene->Registry.remove<T>(Scene::ActiveScene->SelectedEntity.value());
    ImGui::PopStyleVar();
}

template <typename T>
void InspectorPanel::AddComponentEntry(const std::string &name)
{
    auto selectedEntity = Scene::ActiveScene->SelectedEntity;
    if (!Scene::ActiveScene->Registry.any_of<T>(selectedEntity.value()))
    {
        if (ImGui::MenuItem(name.c_str()))
        {
            Scene::ActiveScene->Registry.emplace<T>(selectedEntity.value(), selectedEntity.value());
            ImGui::CloseCurrentPopup();
        }
    }
}
