#pragma once

#include <GLFW/glfw3.h>

#include <d3d11.h>
#include <dxgi1_3.h>
#include <wrl.h>

class DX11Context
{
public:
    DX11Context(GLFWwindow *glfwWindow, const int &width, const int &height);
    ~DX11Context();

    const Microsoft::WRL::ComPtr<ID3D11Device> &GetDevice() const;
    const Microsoft::WRL::ComPtr<ID3D11DeviceContext> &GetDeviceContext() const;

    void OnResize(const int &width, const int &height);
    void Render() const;

private:
    int _width;
    int _height;

    Microsoft::WRL::ComPtr<IDXGIFactory2> _dxgiFactory;
    Microsoft::WRL::ComPtr<ID3D11Device> _device;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> _deviceContext;
    Microsoft::WRL::ComPtr<IDXGISwapChain1> _swapChain;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> _renderTargetView;

    void CreateSwapChain();
    void DeleteSwapChain();
};
