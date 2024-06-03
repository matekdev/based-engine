#pragma once

#include "dx11_context.hpp"

#include <GLFW/glfw3.h>

#include <string>
#include <memory>

class Application
{
public:
    Application(const int &width, const int &height, const std::string &title);
    ~Application();

    void Run() const;

protected:
    void OnResize(const int32_t width, const int32_t height);

private:
    GLFWwindow *_glfwWindow;
    std::unique_ptr<DX11Context> _dx11Context;

    int _width;
    int _height;

    inline static void ResizeCallback(GLFWwindow *window, const int32_t width, const int32_t height);
};
