#include "inspector_panel.hpp"

#include "scene.hpp"
#include "icons/regular.hpp"
#include "icons/brands.hpp"

#include "components/info_component.hpp"
#include "components/transform_component.hpp"

#include "common/file_util.hpp"

#include <imgui.h>
#include <imgui_stdlib.h>
#include <glm/gtc/type_ptr.hpp>
#include <filesystem>

InspectorPanel::InspectorPanel()
{
    for (const auto &entry : std::filesystem::recursive_directory_iterator("models"))
    {
        // objs seem to import the best, but assimp supports everything so feel free to modify this.
        if (entry.is_regular_file() && entry.path().extension() == ".obj")
            _modelPaths.push_back(entry.path().string());
    }
}

void InspectorPanel::Draw()
{
    ImGui::Begin("Inspector");

    auto selectedEntity = Scene::ActiveScene->SelectedEntity;
    if (!selectedEntity.has_value())
    {
        ImGui::End();
        return;
    }

    ComponentHeader<TransformComponent>(
        ICON_FA_UNITY " Entity",
        [this](TransformComponent *transform)
        {
            auto selectedEntity = Scene::ActiveScene->SelectedEntity;
            auto info = Scene::ActiveScene->Registry.try_get<InfoComponent>(selectedEntity.value());
            ImGui::PushItemWidth(-1);
            ImGui::InputText("##label", &info->Name);
            ImGui::PopItemWidth();

            auto floatMin = std::numeric_limits<float>::min();
            auto floatMax = -std::numeric_limits<float>::max();
            ImGui::DragFloat3(ICON_FA_ARROW_UP_RIGHT_FROM_SQUARE " Position", glm::value_ptr(transform->Position), 0.05f, floatMin, floatMax);
            ImGui::DragFloat3(ICON_FA_ARROWS_ROTATE " Rotation", glm::value_ptr(transform->Rotation), 0.05f, floatMin, floatMax);
            ImGui::DragFloat3(ICON_FA_MAGNIFYING_GLASS_PLUS " Scale", glm::value_ptr(transform->Scale), 0.05f, floatMin, floatMax);
        },
        false);

    ComponentHeader<ModelComponent>(
        ICON_FA_PERSON " Model",
        [this](ModelComponent *model)
        {
            auto loadedPath = model->GetLoadedModelPath();
            auto selectedModelName = GetFileName(loadedPath);
            if (ImGui::BeginCombo(ICON_FA_FOLDER_OPEN " Model", selectedModelName.c_str()))
            {
                for (const auto &path : _modelPaths)
                {
                    if (ImGui::Selectable(GetFileName(path).c_str(), loadedPath == path))
                        model->LoadModel(path);
                }

                ImGui::EndCombo();
            }
        });

    ComponentEntries();

    ImGui::End();
}

void InspectorPanel::ComponentEntries()
{
    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::Spacing();

    if (ImGui::Button(ICON_FA_CIRCLE_PLUS " Add Component", ImVec2(-1, 0)))
        ImGui::OpenPopup("AddComponent");

    if (ImGui::BeginPopup("AddComponent"))
    {
        ComponentEntry<ModelComponent>(ICON_FA_PERSON " Model");
        ImGui::EndPopup();
    }
}

template <typename T>
void InspectorPanel::ComponentHeader(const std::string &name, const std::function<void(T *)> &options, const bool &canDelete)
{
    auto flags = ImGuiTreeNodeFlags_AllowOverlap | ImGuiTreeNodeFlags_DefaultOpen;
    auto selectedEntity = Scene::ActiveScene->SelectedEntity;

    auto component = Scene::ActiveScene->Registry.try_get<T>(selectedEntity.value());
    if (component)
    {
        auto info = Scene::ActiveScene->Registry.try_get<InfoComponent>(selectedEntity.value());
        auto isOpen = ImGui::CollapsingHeader(name.c_str(), flags);

        ImGui::PushID(info->GetId());

        if (canDelete)
        {
            ImGui::SameLine();
            ImGui::SetCursorPosX(ImGui::GetWindowWidth() - ImGui::GetStyle().ItemSpacing.x - ImGui::GetFrameHeight());
            ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 18.0f);
            if (ImGui::Button(ICON_FA_XMARK))
                Scene::ActiveScene->Registry.remove<T>(Scene::ActiveScene->SelectedEntity.value());
            ImGui::PopStyleVar();
        }

        if (isOpen)
            options(component);

        ImGui::PopID();
    }
}

template <typename T>
void InspectorPanel::ComponentEntry(const std::string &name)
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
