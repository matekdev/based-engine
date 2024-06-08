#pragma once

#include "scene.hpp"
#include "render/dx11_context.hpp"

#include "ui/ui_context.hpp"
#include "ui/scene_panel.hpp"
#include "ui/console_panel.hpp"

#include <GLFW/glfw3.h>

#include <string>
#include <memory>

class Application
{
public:
    Application(const int &width, const int &height, const std::string &title);
    ~Application();

    void Run() const;

private:
    GLFWwindow *_glfwWindow;
    std::unique_ptr<DX11Context> _dx11Context;
    std::unique_ptr<UIContext> _uiContext;

    std::unique_ptr<Scene> _scene;
    std::unique_ptr<ScenePanel> _scenePanel;
    std::unique_ptr<ConsolePanel> _consolePanel;

    int _width;
    int _height;

    inline static void ResizeCallback(GLFWwindow *window, const int32_t width, const int32_t height);
    void OnResize(const int32_t width, const int32_t height);
};
