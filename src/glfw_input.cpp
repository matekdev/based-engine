#include "glfw_input.hpp"

#include "window.hpp"

glm::vec2 GLFWInput::GetMousePosition()
{
    auto *window = static_cast<GLFWwindow *>(Window::GetNativeWindow());
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    return {(float)xpos, (float)ypos};
}

void GLFWInput::OnMouseClick(GLFWwindow *glfwWindow, int button, int action, int mods)
{
    Window *window = static_cast<Window *>(glfwGetWindowUserPointer(glfwWindow));
    if (window)
        window->OnMouseClick(button, action, mods);
}
