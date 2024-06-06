#pragma once

#include <GLFW/glfw3.h>

#include <d3d11.h>
#include <dxgi1_3.h>
#include <wrl.h>

#include <string>

class DX11Context
{
public:
    DX11Context(GLFWwindow *glfwWindow);
    ~DX11Context();

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

    // TODO: Move this stuff into seperate files probably
    Microsoft::WRL::ComPtr<ID3D11InputLayout> _inputLayout;
    Microsoft::WRL::ComPtr<ID3D11VertexShader> _vertexShader = nullptr;
    Microsoft::WRL::ComPtr<ID3D11PixelShader> _pixelShader = nullptr;
    Microsoft::WRL::ComPtr<ID3D11Buffer> _triangleVertices = nullptr;

    void InitializeShaders();
};
