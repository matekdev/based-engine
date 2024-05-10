#pragma once

#include <glm/glm.hpp>

struct GLFWwindow;

class GLFWInput
{
public:
    static glm::vec2 GetMousePosition();
    static void OnMouseClick(GLFWwindow *glfwWindow, int button, int action, int mods);
};
