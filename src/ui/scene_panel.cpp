#include "scene_panel.hpp"

#include "scene.hpp"

#include "imgui.h"
#include "log.hpp"

const float &ScenePanel::GetWidth()
{
    return _width;
}

const float &ScenePanel::GetHeight()
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

    auto panelSize = ImGui::GetContentRegionAvail();
    if (panelSize.x != _width || panelSize.y != _height)
    {
        _width = panelSize.x;
        _height = panelSize.y;

        Scene::ActiveScene->OnResize();
    }

    ImGui::Image(Scene::ActiveScene->GetShaderResourceView(), panelSize);

    ImGui::End();
}
