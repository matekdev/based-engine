#include "application.hpp"

#include <stdexcept>

Application::Application(const int &width, const int &height, const std::string &windowTitle) : _width(width), _height(height)
{
    if (!glfwInit())
        throw std::runtime_error("Failed to create GLFW Window.");

    glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_FALSE);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    _glfwWindow = glfwCreateWindow(width, height, windowTitle.c_str(), nullptr, nullptr);
    if (!_glfwWindow)
        throw std::runtime_error("Failed to create GLFW Window.");

    glfwSetWindowUserPointer(_glfwWindow, this);
    glfwMakeContextCurrent(_glfwWindow);
    glfwSetFramebufferSizeCallback(_glfwWindow, ResizeCallback);

    // Create it early so we can access logs.
    _consolePanel = std::make_unique<ConsolePanel>();

    _dx11Context = std::make_unique<DX11Context>(_glfwWindow, height, width);
    _uiContext = std::make_unique<UIContext>(_glfwWindow, _dx11Context->GetDevice().Get(), _dx11Context->GetDeviceContext().Get());
    _scenePanel = std::make_unique<ScenePanel>();
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
        _uiContext->PreRender();

        // _dx11Context->Render();

        _scenePanel->Render();
        _consolePanel->Render();

        _uiContext->PostRender();

        glfwPollEvents();
    }
}

void Application::ResizeCallback(GLFWwindow *window, const int32_t width, const int32_t height)
{
    const auto *application = static_cast<Application *>(glfwGetWindowUserPointer(window));
    if (application)
        application->OnResize(width, height);
}

void Application::OnResize(const int32_t width, const int32_t height) const
{
    _dx11Context->OnResize(width, height);
}
