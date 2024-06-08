#include "renderer.hpp"

#include "vertex.hpp"
#include "log.hpp"

#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WGL
#define GLFW_NATIVE_INCLUDE_NONE
#include <GLFW/glfw3native.h>

#include <d3dcompiler.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

#include <stddef.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_dx11.h>

Renderer::Renderer(GLFWwindow *glfwWindow)
{
    if (FAILED(CreateDXGIFactory1(IID_PPV_ARGS(&_dxgiFactory))))
    {
        LOG(ERROR) << "Failed to create DXGIFactory";
        return;
    }

    if (FAILED(CreateDXGIFactory1(IID_PPV_ARGS(_dxgiFactory.GetAddressOf()))))
    {
        LOG(ERROR) << "Failed to create DXGIFactory";
        return;
    }

    D3D_FEATURE_LEVEL deviceFeatureLevel = D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_11_0;
    if (FAILED(D3D11CreateDevice(
            nullptr,
            D3D_DRIVER_TYPE::D3D_DRIVER_TYPE_HARDWARE,
            nullptr,
            0,
            &deviceFeatureLevel,
            1,
            D3D11_SDK_VERSION,
            _device.GetAddressOf(),
            nullptr,
            _deviceContext.GetAddressOf())))
    {
        LOG(ERROR) << "Failed to create DXGIDevice";
        return;
    }

    DXGI_SWAP_CHAIN_DESC1 swapChainDescriptor = {};
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

    if (FAILED(_dxgiFactory->CreateSwapChainForHwnd(
            _device.Get(),
            glfwGetWin32Window(glfwWindow),
            &swapChainDescriptor,
            &swapChainFullscreenDescriptor,
            nullptr,
            _swapChain.GetAddressOf())))
    {
        LOG(INFO) << "Failed to create swap chain";
        return;
    }

    CreateSwapChain();
    InitializeShaders();
    CreateVertexBuffers();
}

Renderer::~Renderer()
{
    _deviceContext->Flush();
    DeleteSwapChain();
    _swapChain.Reset();
    _dxgiFactory.Reset();
    _deviceContext.Reset();
    _device.Reset();
}

const Microsoft::WRL::ComPtr<ID3D11Device> &Renderer::GetDevice()
{
    return _device;
}

const Microsoft::WRL::ComPtr<ID3D11DeviceContext> &Renderer::GetDeviceContext()
{
    return _deviceContext;
}

void Renderer::OnResize(const int &width, const int &height)
{
    _width = width;
    _height = height;

    _deviceContext->Flush();
    DeleteSwapChain();
    _swapChain->ResizeBuffers(0, width, height, DXGI_FORMAT::DXGI_FORMAT_B8G8R8A8_UNORM, 0);
    CreateSwapChain();
}

void Renderer::PreRender() const
{
    D3D11_VIEWPORT viewport = {};
    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.Width = static_cast<float>(_width);
    viewport.Height = static_cast<float>(_height);
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;

    constexpr float clearColor[] = {0.1f, 0.1f, 0.1f, 1.0f};
    constexpr UINT vertexStride = sizeof(VertexPositionColor);
    constexpr UINT vertexOffset = 0;

    _deviceContext->ClearRenderTargetView(_renderTargetView.Get(), clearColor);

    _deviceContext->IASetVertexBuffers(0, 1, _triangleVertices.GetAddressOf(), &vertexStride, &vertexOffset);
    _deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    _vertexShader->Bind(_deviceContext);

    _deviceContext->RSSetViewports(1, &viewport);

    _pixelShader->Bind(_deviceContext);

    _deviceContext->OMSetRenderTargets(1, _renderTargetView.GetAddressOf(), nullptr);
    _deviceContext->Draw(3, 0);
}

void Renderer::PostRender() const
{
    _swapChain->Present(1, 0);
}

void Renderer::CreateSwapChain()
{
    Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;
    _swapChain->GetBuffer(0, IID_PPV_ARGS(backBuffer.GetAddressOf()));
    _device->CreateRenderTargetView(backBuffer.Get(), nullptr, _renderTargetView.GetAddressOf());
}

void Renderer::DeleteSwapChain()
{
    _renderTargetView.Reset();
}

void Renderer::InitializeShaders()
{
    _vertexShader = std::make_unique<VertexShader>(_device, L"shaders/model.vs.hlsl");
    _pixelShader = std::make_unique<PixelShader>(_device, L"shaders/model.ps.hlsl");
}

void Renderer::CreateVertexBuffers()
{
    constexpr VertexPositionColor vertices[] = {
        {DirectX::XMFLOAT3{0.0f, 0.5f, 0.0f}, DirectX::XMFLOAT3{0.25f, 0.39f, 0.19f}},
        {DirectX::XMFLOAT3{0.5f, -0.5f, 0.0f}, DirectX::XMFLOAT3{0.44f, 0.75f, 0.35f}},
        {DirectX::XMFLOAT3{-0.5f, -0.5f, 0.0f}, DirectX::XMFLOAT3{0.38f, 0.55f, 0.20f}},
    };
    D3D11_BUFFER_DESC bufferInfo = {};
    bufferInfo.ByteWidth = sizeof(vertices);
    bufferInfo.Usage = D3D11_USAGE::D3D11_USAGE_IMMUTABLE;
    bufferInfo.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;

    D3D11_SUBRESOURCE_DATA resourceData = {};
    resourceData.pSysMem = vertices;

    if (FAILED(_device->CreateBuffer(
            &bufferInfo,
            &resourceData,
            &_triangleVertices)))
        LOG(ERROR) << "Failed to create vertex buffer";
}
