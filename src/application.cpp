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

    _renderer = std::make_unique<Renderer>(_glfwWindow);
    _renderer->OnResize(width, height);

    _scene = std::make_unique<Scene>();
    _scenePanel = std::make_unique<ScenePanel>();
    _uiContext = std::make_unique<UIContext>(_glfwWindow, _renderer->GetDevice().Get(), _renderer->GetDeviceContext().Get());
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
        _renderer->PreRender();

        _scene->Render();

        _consolePanel->Render();

        _uiContext->PostRender();
        _renderer->PostRender();

        // TODO: Move to scene panel?
        _scene->GetCamera().Update(_width, _height, _glfwWindow);

        glfwPollEvents();
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
    _width = width;
    _height = height;

    _renderer->OnResize(width, height);
}
