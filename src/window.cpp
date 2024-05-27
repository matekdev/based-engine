#include "window.hpp"

#include "scene.hpp"
#include "glfw_input.hpp"

Window::Window(int width, int height, const std::string &windowTitle) : _width(width), _height(height)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    _glfwWindow = glfwCreateWindow(width, height, windowTitle.c_str(), nullptr, nullptr);
    if (!_glfwWindow)
        throw std::runtime_error("Failed to create GLFW Window.");

    glfwSetWindowUserPointer(_glfwWindow, this);
    glfwSetMouseButtonCallback(_glfwWindow, GLFWInput::OnMouseClick);
    glfwMakeContextCurrent(_glfwWindow);
    gladLoadGL();

    glEnable(GL_DEPTH_TEST);

    glEnable(GL_STENCIL_TEST);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    _glContext = std::make_unique<OpenGLContext>(this);
    _uiContext = std::make_unique<UIContext>(this);
    _scene = std::make_unique<Scene>();

    _scenePanel = std::make_unique<ScenePanel>();
    _consolePanel = std::make_unique<ConsolePanel>();
    _entitiesPanel = std::make_unique<EntitiesPanel>();
    _inspectorPanel = std::make_unique<InspectorPanel>();
}

Window::~Window()
{
    _uiContext->Destroy();
    _glContext->Destroy();
}

void Window::Run()
{
    while (!glfwWindowShouldClose(_glfwWindow))
    {
        _glContext->PreRender();
        _uiContext->PreRender();

        _scene->Render(GetNativeWindow());

        _scenePanel->Render(_glfwWindow);
        _scenePanel->Input(_glfwWindow);

        _consolePanel->Render();
        _entitiesPanel->Render();
        _inspectorPanel->Render();

        _uiContext->PostRender();
        _glContext->PostRender();
    }
}

GLFWwindow *Window::GetNativeWindow()
{
    return _glfwWindow;
}

int Window::GetWidth()
{
    return _width;
}

int Window::GetHeight()
{
    return _height;
}

void Window::OnMouseClick(int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
        _scenePanel->OnMouseClick();
}
