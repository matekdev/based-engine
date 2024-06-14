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
