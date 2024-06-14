#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class GLFWUtil
{
public:
    static GLFWwindow *GetNativeWindow();
    static glm::vec2 GetWindowSize();
    static glm::vec2 GetMousePosition();
};
