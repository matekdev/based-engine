#include "renderer.hpp"

#include "log.hpp"
#include "ui/imgui_config.hpp"

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

Renderer::Renderer(GLFWwindow *glfwWindow, const int &width, const int &height) : _glfwWindow(glfwWindow), _width(width), _height(height)
{
    InitializeFactoryAndDevice();
    InitializeSwapChain();
    InitializeShaders();
    InitializeImGui();
    InitializeBackBuffer();
}

Renderer::~Renderer()
{
    _deviceContext->Flush();
    _renderTargetView.Reset();
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
    _renderTargetView.Reset();
    _swapChain->ResizeBuffers(0, width, height, DXGI_FORMAT::DXGI_FORMAT_B8G8R8A8_UNORM, 0);
    InitializeBackBuffer();
}

void Renderer::PreRender() const
{
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    D3D11_VIEWPORT viewport = {};
    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.Width = static_cast<float>(_width);
    viewport.Height = static_cast<float>(_height);
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;

    constexpr float clearColor[] = {0.1f, 0.1f, 0.1f, 1.0f};
    _deviceContext->ClearRenderTargetView(_renderTargetView.Get(), clearColor);

    // TODO: Move over to model
    // _deviceContext->IASetVertexBuffers(0, 1, _triangleVertices.GetAddressOf(), &vertexStride, &vertexOffset);
    // _deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    _vertexShader->Bind(_deviceContext);

    _deviceContext->RSSetViewports(1, &viewport);

    _pixelShader->Bind(_deviceContext);

    _deviceContext->OMSetRenderTargets(1, _renderTargetView.GetAddressOf(), nullptr);
    _deviceContext->Draw(3, 0);
}

void Renderer::PostRender() const
{
    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    ImGuiIO &io = ImGui::GetIO();

    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        GLFWwindow *backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }

    _swapChain->Present(1, 0);
}

void Renderer::InitializeFactoryAndDevice()
{
    if (FAILED(CreateDXGIFactory1(IID_PPV_ARGS(&_dxgiFactory))))
        LOG(ERROR) << "Failed to create DXGIFactory";

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
        LOG(ERROR) << "Failed to create DXGIDevice";
}

void Renderer::InitializeSwapChain()
{
    DXGI_SWAP_CHAIN_DESC1 swapChainDescriptor = {};
    swapChainDescriptor.Format = DXGI_FORMAT::DXGI_FORMAT_B8G8R8A8_UNORM;
    swapChainDescriptor.Width = _width;
    swapChainDescriptor.Height = _height;
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
            glfwGetWin32Window(_glfwWindow),
            &swapChainDescriptor,
            &swapChainFullscreenDescriptor,
            nullptr,
            _swapChain.GetAddressOf())))
        LOG(ERROR) << "Failed to create swap chain";
}

void Renderer::InitializeShaders()
{
    _vertexShader = std::make_unique<VertexShader>(_device, L"shaders/model.vs.hlsl");
    _pixelShader = std::make_unique<PixelShader>(_device, L"shaders/model.ps.hlsl");
}

void Renderer::InitializeImGui()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiConfig::Load();

    ImGui_ImplGlfw_InitForOther(_glfwWindow, true);
    ImGui_ImplDX11_Init(_device.Get(), _deviceContext.Get());
}

void Renderer::InitializeBackBuffer()
{
    Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;
    _swapChain->GetBuffer(0, IID_PPV_ARGS(backBuffer.GetAddressOf()));
    _device->CreateRenderTargetView(backBuffer.Get(), nullptr, _renderTargetView.GetAddressOf());
}
