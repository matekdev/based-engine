#include "inspector_panel.hpp"

#include "game.hpp"
#include "components/vec3_control.hpp"

#include "imgui.h"
#include "imgui_stdlib.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <glm/gtc/type_ptr.hpp>
#include <limits>

InspectorPanel::InspectorPanel()
{
}

void InspectorPanel::Render()
{
    ImGui::Begin("Inspector");

    if (Game::SelectedGameObject)
    {
        if (ImGui::CollapsingHeader("Rendering", ImGuiTreeNodeFlags_DefaultOpen))
        {
            const char *renderingModes[] = {"Regular", "Wireframe", "Points"};
            ImGui::Combo("##label", &Game::SelectedGameObject->RenderingMode, renderingModes, IM_ARRAYSIZE(renderingModes), IM_ARRAYSIZE(renderingModes));
            ImGui::ColorEdit3("Ambient", glm::value_ptr(Game::SelectedGameObject->Ambient));
            ImGui::ColorEdit3("Diffuse", glm::value_ptr(Game::SelectedGameObject->Diffuse));
            ImGui::ColorEdit3("Specular", glm::value_ptr(Game::SelectedGameObject->Specular));

            if (Game::SelectedGameObject->GetType() == GameObject::Type::Model)
                ImGui::DragFloat("Shininess", &Game::SelectedGameObject->Shininess, 1.0f, 1.0f, 500.0f);

            if (Game::SelectedGameObject->GetType() == GameObject::Type::Light)
                ImGui::DragFloat("Attenuation", &Game::SelectedGameObject->Attenuation, 0.05f, 0.01f, 10.0f);
        }

        ImGui::Spacing();

        if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
        {
            auto floatMin = std::numeric_limits<float>::min();
            auto floatMax = -std::numeric_limits<float>::max();

            DrawVec3Control("Position", Game::SelectedGameObject->Position, 0.0f, 100.0f);
            DrawVec3Control("Rotation", Game::SelectedGameObject->Rotation, 0.0f, 100.0f);
            DrawVec3Control("Scale", Game::SelectedGameObject->Scale, 1.0f, 100.0f);
        }

        ImGui::Spacing();

        if (ImGui::CollapsingHeader("Stats", ImGuiTreeNodeFlags_DefaultOpen))
        {
            ImGui::Text("Vertices: %d", Game::SelectedGameObject->GetVertexCount());
            ImGui::Text("Triangles: %d", Game::SelectedGameObject->GetTriangleCount());
        }
    }

    ImGui::End();
}
