#include "objects_panel.hpp"

#include "game.hpp"

#include "imgui.h"
#include "imgui_stdlib.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "log.hpp"

#include <string>
#include <filesystem>

ObjectsPanel::ObjectsPanel()
{
    for (const auto &entry : std::filesystem::recursive_directory_iterator("models"))
    {
        // objs seem to import the best, but assimp supports everything so feel free to modify this.
        if (entry.is_regular_file() && entry.path().extension() == ".obj")
            _modelPaths.push_back(entry.path().string());
    }
}

void ObjectsPanel::Render()
{
    ImGui::Begin("Objects");

    if (ImGui::Button("Create GameObject", ImVec2(ImGui::GetContentRegionMax().x, 30.0f)))
        ImGui::OpenPopup("create_menu");
    if (ImGui::BeginPopup("create_menu"))
    {
        for (auto &path : _modelPaths)
        {
            auto fileName = path.substr(path.find_last_of("\\") + 1);
            if (ImGui::Button(fileName.c_str(), ImVec2(ImGui::GetContentRegionAvail().x, 20.0f)))
            {
                // Unique case for light, this could be written better!
                auto isLight = path.contains("light");
                Game::GameObjects.push_back(GameObject(path, isLight ? GameObject::Type::Light : GameObject::Type::Model));
                Game::SelectedGameObject = &Game::GameObjects.back();
                ImGui::CloseCurrentPopup();
            }
        }
        ImGui::EndPopup();
    }

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    for (auto &gameObject : Game::GameObjects)
    {
        if (ImGui::Selectable(gameObject.Name.c_str(), Game::SelectedGameObject == &gameObject))
        {
            Game::SelectedGameObject = &gameObject;
        }
    }

    ImGui::End();
}
