#pragma once

#include <GLFW/glfw3.h>

#include <d3d11.h>
#include <wrl.h>

class UIContext
{
public:
    UIContext(GLFWwindow *glfwWindow,
              const Microsoft::WRL::ComPtr<ID3D11Device> &device,
              const Microsoft::WRL::ComPtr<ID3D11DeviceContext> &deviceContext);
    ~UIContext();

    void PreRender() const;
    void PostRender() const;
};
