#include "application.hpp"

Application::Application(int width, int height, const std::string &windowTitle) : _width(width), _height(height)
{
    glfwInit();

    glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_FALSE);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    _glfwWindow = glfwCreateWindow(width, height, windowTitle.c_str(), nullptr, nullptr);
    if (!_glfwWindow)
        throw std::runtime_error("Failed to create GLFW Window.");

    glfwSetWindowUserPointer(_glfwWindow, this);
    glfwMakeContextCurrent(_glfwWindow);
}

Application::~Application()
{
    glfwDestroyWindow(_glfwWindow);
    glfwTerminate();
}

void Application::Run() const
{
    while (!glfwWindowShouldClose(_glfwWindow))
    {
        glfwPollEvents();
    }
}
