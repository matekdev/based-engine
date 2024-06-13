#pragma once

#include "scene.hpp"
#include "render/renderer.hpp"

#include "ui/panel.hpp"

#include <GLFW/glfw3.h>

#include <string>
#include <memory>
#include <vector>

class Application
{
public:
    Application(const int &width, const int &height, const std::string &title);
    ~Application();

    void Run() const;
    void DrawPanels() const;

private:
    GLFWwindow *_glfwWindow;
    std::unique_ptr<Renderer> _renderer;
    std::unique_ptr<Scene> _scene;

    std::vector<std::unique_ptr<Panel>> _uiPanels;

    int _width;
    int _height;

    inline static void ResizeCallback(GLFWwindow *window, const int32_t width, const int32_t height);
    void OnResize(const int32_t width, const int32_t height);
};
