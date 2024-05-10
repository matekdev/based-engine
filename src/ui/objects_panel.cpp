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

    ImGui::End();
}
