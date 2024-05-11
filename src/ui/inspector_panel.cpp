#include "inspector_panel.hpp"

#include "scene.hpp"
#include "ui/components/vec3_control.hpp"

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
        auto transform = Scene::ActiveScene->Registry.try_get<TransformComponent>(selectedEntity.value());
        if (transform && ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
        {
            auto floatMin = std::numeric_limits<float>::min();
            auto floatMax = -std::numeric_limits<float>::max();

            DrawVec3Control("Position", transform->Position, 0.0f, 100.0f);
            DrawVec3Control("Rotation", transform->Rotation, 0.0f, 100.0f);
            DrawVec3Control("Scale", transform->Scale, 1.0f, 100.0f);
        }
    }

    ImGui::End();
}
