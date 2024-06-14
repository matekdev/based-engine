#include "glfw_util.hpp"

#include "application.hpp"

GLFWwindow *GLFWUtil::GetNativeWindow()
{
    return Application::GetNativeWindow();
}

glm::vec2 GLFWUtil::GetWindowSize()
{
    int width, height;
    glfwGetWindowSize(Application::GetNativeWindow(), &width, &height);
    return glm::vec2{width, height};
}

glm::vec2 GLFWUtil::GetMousePosition()
{
    double xpos, ypos;
    glfwGetCursorPos(Application::GetNativeWindow(), &xpos, &ypos);
    return {xpos, ypos};
}

bool GLFWUtil::IsButtonPressed(int key)
{
    return glfwGetKey(Application::GetNativeWindow(), key) == GLFW_PRESS;
}

bool GLFWUtil::IsMouseLocked()
{
    auto inputMode = glfwGetInputMode(Application::GetNativeWindow(), GLFW_CURSOR);
    return inputMode == GLFW_CURSOR_DISABLED;
}

void GLFWUtil::SetMouseLock(bool isLocked)
{
    glfwSetInputMode(Application::GetNativeWindow(), GLFW_CURSOR, isLocked ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
}
