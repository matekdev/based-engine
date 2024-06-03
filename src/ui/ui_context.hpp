#pragma once

#include <GLFW/glfw3.h>

#include <d3d11.h>

class UIContext
{
public:
    UIContext(GLFWwindow *glfwWindow, ID3D11Device *device, ID3D11DeviceContext *deviceContext);
    ~UIContext();

    void PreRender();
    void PostRender();
};
