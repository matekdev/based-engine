#pragma once

#include "render/vertex_shader.hpp"
#include "render/pixel_shader.hpp"

#include <GLFW/glfw3.h>

#include <d3d11.h>
#include <dxgi1_3.h>
#include <wrl.h>

#include <string>
#include <memory>

class Renderer
{
public:
    Renderer(GLFWwindow *glfwWindow);
    ~Renderer();

    static const Microsoft::WRL::ComPtr<ID3D11Device> &GetDevice();
    static const Microsoft::WRL::ComPtr<ID3D11DeviceContext> &GetDeviceContext();

    void OnResize(const int &width, const int &height);
    void PreRender() const;
    void PostRender() const;

private:
    int _width;
    int _height;

    Microsoft::WRL::ComPtr<IDXGIFactory2> _dxgiFactory;
    static inline Microsoft::WRL::ComPtr<ID3D11Device> _device;
    static inline Microsoft::WRL::ComPtr<ID3D11DeviceContext> _deviceContext;
    Microsoft::WRL::ComPtr<IDXGISwapChain1> _swapChain;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> _renderTargetView;

    void CreateSwapChain();
    void DeleteSwapChain();

    std::unique_ptr<VertexShader> _vertexShader;
    std::unique_ptr<PixelShader> _pixelShader;
    Microsoft::WRL::ComPtr<ID3D11Buffer> _triangleVertices = nullptr;

    void InitializeShaders();
    void CreateVertexBuffers();
};
