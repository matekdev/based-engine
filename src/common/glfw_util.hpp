#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class GLFWUtil
{
public:
    static GLFWwindow *GetNativeWindow();
    static glm::vec2 GetWindowSize();
    static glm::vec2 GetMousePosition();
    static bool IsMouseButtonPressed(int key);
    static bool IsButtonPressed(int key);
    static bool IsMouseLocked();
    static void SetMouseLock(bool isLocked);
};
