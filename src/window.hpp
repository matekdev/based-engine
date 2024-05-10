#pragma once

#include "render/opengl_context.hpp"
#include "render/ui_context.hpp"

#include "ui/scene_panel.hpp"
#include "ui/console_panel.hpp"
#include "ui/objects_panel.hpp"
#include "ui/inspector_panel.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <memory>

class Window
{
public:
    Window(int width, int height, const std::string &title);
    ~Window();

    void Run();
    static GLFWwindow *GetNativeWindow();
    int GetWidth();
    int GetHeight();
    void OnMouseClick(int button, int action, int mods);

private:
    inline static GLFWwindow *_glfwWindow;
    std::unique_ptr<OpenGLContext> _glContext;
    std::unique_ptr<UIContext> _uiContext;

    std::unique_ptr<ScenePanel> _scenePanel;
    std::unique_ptr<ConsolePanel> _consolePanel;
    std::unique_ptr<ObjectsPanel> _objectsPanel;
    std::unique_ptr<InspectorPanel> _inspectorPanel;

    int _width;
    int _height;
};