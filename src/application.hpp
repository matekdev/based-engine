#pragma once

#include "scene.hpp"
#include "render/renderer.hpp"

#include "ui/panel.hpp"

#include <GLFW/glfw3.h>
#include <PxPhysicsAPI.h>

#include <string>
#include <memory>
#include <vector>

class Application
{
public:
    Application(const float &width, const float &height, const std::string &title);
    ~Application();

    static GLFWwindow *GetNativeWindow();
    void Run() const;
    void DrawPanels() const;

private:
    static inline GLFWwindow *_glfwWindow;
    std::unique_ptr<Renderer> _renderer;
    std::unique_ptr<Scene> _scene;

    std::vector<std::unique_ptr<Panel>> _uiPanels;

    inline static void ResizeCallback(GLFWwindow *window, const int32_t width, const int32_t height);
    void OnResize(const int32_t width, const int32_t height);
};
