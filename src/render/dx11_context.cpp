#include "dx11_context.hpp"

#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WGL
#define GLFW_NATIVE_INCLUDE_NONE
#include <GLFW/glfw3native.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

DX11Context::DX11Context(GLFWwindow *glfwWindow, const int &width, const int &height) : _width(width), _height(_height)
{
    CreateDXGIFactory1(IID_PPV_ARGS(_dxgiFactory.GetAddressOf()));

    D3D_FEATURE_LEVEL deviceFeatureLevel = D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_11_0;
    D3D11CreateDevice(
        nullptr,
        D3D_DRIVER_TYPE::D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        0,
        &deviceFeatureLevel,
        1,
        D3D11_SDK_VERSION,
        _device.GetAddressOf(),
        nullptr,
        _deviceContext.GetAddressOf());

    DXGI_SWAP_CHAIN_DESC1 swapChainDescriptor = {};
    swapChainDescriptor.Width = width;
    swapChainDescriptor.Height = height;
    swapChainDescriptor.Format = DXGI_FORMAT::DXGI_FORMAT_B8G8R8A8_UNORM;
    swapChainDescriptor.SampleDesc.Count = 1;
    swapChainDescriptor.SampleDesc.Quality = 0;
    swapChainDescriptor.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDescriptor.BufferCount = 2;
    swapChainDescriptor.SwapEffect = DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_FLIP_DISCARD;
    swapChainDescriptor.Scaling = DXGI_SCALING::DXGI_SCALING_STRETCH;
    swapChainDescriptor.Flags = {};

    DXGI_SWAP_CHAIN_FULLSCREEN_DESC swapChainFullscreenDescriptor = {};
    swapChainFullscreenDescriptor.Windowed = true;

    _dxgiFactory->CreateSwapChainForHwnd(
        _device.Get(),
        glfwGetWin32Window(glfwWindow),
        &swapChainDescriptor,
        &swapChainFullscreenDescriptor,
        nullptr,
        _swapChain.GetAddressOf());

    CreateSwapChain();
}

DX11Context::~DX11Context()
{
    _deviceContext->Flush();
    DeleteSwapChain();
    _swapChain.Reset();
    _dxgiFactory.Reset();
    _deviceContext.Reset();
    _device.Reset();
}

Microsoft::WRL::ComPtr<ID3D11Device> DX11Context::GetDevice()
{
    return _device;
}

Microsoft::WRL::ComPtr<ID3D11DeviceContext> DX11Context::GetDeviceContext()
{
    return _deviceContext;
}

void DX11Context::OnResize(const int &width, const int &height)
{
    _width = width;
    _height = height;

    _deviceContext->Flush();
    DeleteSwapChain();
    _swapChain->ResizeBuffers(0, width, height, DXGI_FORMAT::DXGI_FORMAT_B8G8R8A8_UNORM, 0);
    CreateSwapChain();
}

void DX11Context::Render()
{
    D3D11_VIEWPORT viewport = {};
    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.Width = static_cast<float>(_width);
    viewport.Height = static_cast<float>(_width);
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;

    constexpr float clearColor[] = {0.1f, 0.1f, 0.1f, 1.0f};

    _deviceContext->ClearRenderTargetView(_renderTargetView.Get(), clearColor);
    _deviceContext->RSSetViewports(1, &viewport);
    _deviceContext->OMSetRenderTargets(1, _renderTargetView.GetAddressOf(), nullptr);
    _swapChain->Present(1, 0);
}

void DX11Context::CreateSwapChain()
{
    Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer = nullptr;
    _swapChain->GetBuffer(0, IID_PPV_ARGS(backBuffer.GetAddressOf()));
    _device->CreateRenderTargetView(backBuffer.Get(), nullptr, _renderTargetView.GetAddressOf());
}

void DX11Context::DeleteSwapChain()
{
    _renderTargetView.Reset();
}
