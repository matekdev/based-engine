#include "application.hpp"

#include "ui/console_panel.hpp"
#include "ui/scene_panel.hpp"
#include "ui/docking_panel.hpp"
#include "ui/entities_panel.hpp"
#include "ui/inspector_panel.hpp"

#include <imgui.h>

#include <stdexcept>

Application::Application(const float &applicationWidth, const float &applicationHeight, const std::string &windowTitle)
{
    if (!glfwInit())
        throw std::runtime_error("Failed to create GLFW Window.");

    glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_FALSE);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    _glfwWindow = glfwCreateWindow(applicationWidth, applicationHeight, windowTitle.c_str(), nullptr, nullptr);
    if (!_glfwWindow)
        throw std::runtime_error("Failed to create GLFW Window.");

    glfwSetWindowUserPointer(_glfwWindow, this);
    glfwMakeContextCurrent(_glfwWindow);
    glfwSetFramebufferSizeCallback(_glfwWindow, ResizeCallback);

    _uiPanels.push_back(std::make_unique<DockingPanel>());
    _uiPanels.push_back(std::make_unique<ConsolePanel>());
    _uiPanels.push_back(std::make_unique<ScenePanel>());
    _uiPanels.push_back(std::make_unique<EntitiesPanel>());
    _uiPanels.push_back(std::make_unique<InspectorPanel>());

    _renderer = std::make_unique<Renderer>();
    _scene = std::make_unique<Scene>();
}

Application::~Application()
{
    glfwDestroyWindow(_glfwWindow);
    glfwTerminate();
}

GLFWwindow *Application::GetNativeWindow()
{
    return _glfwWindow;
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

void Application::ResizeCallback(GLFWwindow *window, const int width, const int height)
{
    auto *application = static_cast<Application *>(glfwGetWindowUserPointer(window));
    if (application)
        application->OnResize(width, height);
}

void Application::OnResize(const int width, const int height)
{
    _renderer->OnResize();
}
