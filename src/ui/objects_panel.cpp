#include "objects_panel.hpp"

#include "scene.hpp"
#include "component/info_component.hpp"

#include "imgui.h"
#include "imgui_stdlib.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "log.hpp"

#include <string>
#include <filesystem>

ObjectsPanel::ObjectsPanel()
{
}

void ObjectsPanel::Render()
{
    ImGui::Begin("Objects");

    auto group = Scene::ActiveScene->Registry.view<InfoComponent>();
    for (auto entity : group)
    {
        auto info = group.get<InfoComponent>(entity);
        if (ImGui::Selectable(info.Name.c_str(), Scene::ActiveScene->SelectedEntity.has_value() && Scene::ActiveScene->SelectedEntity.value() == entity))
        {
            Scene::ActiveScene->SelectedEntity = entity;
        }
    }

    ImGui::End();
}
