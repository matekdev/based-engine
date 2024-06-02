#pragma once

#include <GLFW/glfw3.h>

#include <iostream>
#include <memory>

class Application
{
public:
    Application(int width, int height, const std::string &title);
    ~Application();

    void Run() const;

private:
    inline static GLFWwindow *_glfwWindow;

    int _width;
    int _height;
};