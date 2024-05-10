#pragma once

#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_stdlib.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <string>
#include <glm/glm.hpp>

void DrawVec3Control(const std::string &label, glm::vec3 &values, float resetValue, float columnWidth);