#include "application.hpp"

#include "ui/console_panel.hpp"
#include "ui/scene_panel.hpp"
#include "ui/docking_panel.hpp"

#include <imgui.h>

#include <stdexcept>

Application::Application(const float &width, const float &height, const std::string &windowTitle)
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

    // These panels need to be created early.
    _uiPanels.push_back(std::make_unique<DockingPanel>());
    _uiPanels.push_back(std::make_unique<ConsolePanel>());

    _renderer = std::make_unique<Renderer>(_glfwWindow, width, height);
    _scene = std::make_unique<Scene>();

    _uiPanels.push_back(std::make_unique<ScenePanel>());
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
        _renderer->PreRender();

        _scene->Render();

        _renderer->BindBackBuffer();
        DrawPanels();

        _renderer->PostRender();

        glfwPollEvents();
    }
}

void Application::DrawPanels() const
{
    for (auto &panel : _uiPanels)
    {
        panel->Draw();
    }
}

void Application::ResizeCallback(GLFWwindow *window, const int32_t width, const int32_t height)
{
    auto *application = static_cast<Application *>(glfwGetWindowUserPointer(window));
    if (application)
        application->OnResize(width, height);
}

void Application::OnResize(const int32_t width, const int32_t height)
{
    _renderer->OnResize(width, height);
}
