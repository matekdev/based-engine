#include "inspector_panel.hpp"

#include "scene.hpp"
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

    if (Scene::SelectedGameObject)
    {
    }

    ImGui::End();
}
