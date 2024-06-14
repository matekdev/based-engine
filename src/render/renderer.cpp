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

Renderer::Renderer(GLFWwindow *glfwWindow, const float &width, const float &height) : _glfwWindow(glfwWindow), _width(width), _height(height)
{
    _instance = this;

    InitializeFactoryAndDevice();
    InitializeSwapChain();
    InitializeImGui();
    InitializeBackBuffer();
    SetViewPort();
}

Renderer::~Renderer()
{
    _deviceContext->Flush();
    _backBuffer.Reset();
    _swapChain.Reset();
    _dxgiFactory.Reset();
    _deviceContext.Reset();
    _device.Reset();
}

const Microsoft::WRL::ComPtr<ID3D11Device> &Renderer::GetDevice()
{
    return _instance->_device;
}

const Microsoft::WRL::ComPtr<ID3D11DeviceContext> &Renderer::GetDeviceContext()
{
    return _instance->_deviceContext;
}

GLFWwindow *Renderer::GetNativeWindow()
{
    return _instance->_glfwWindow;
}

void Renderer::OnResize(const float &width, const float &height)
{
    _instance->_width = width;
    _instance->_height = height;
    _instance->_deviceContext->Flush();
    _instance->_backBuffer.Reset();
    _instance->_swapChain->ResizeBuffers(0, _instance->_width, _instance->_height, DXGI_FORMAT::DXGI_FORMAT_B8G8R8A8_UNORM, 0);
    _instance->InitializeBackBuffer();
    _instance->SetViewPort();
}

void Renderer::BindBackBuffer() const
{
    _deviceContext->OMSetRenderTargets(1, _backBuffer.GetAddressOf(), nullptr);
    _deviceContext->ClearRenderTargetView(_backBuffer.Get(), _clearColor);
}

void Renderer::PreRender() const
{
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
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
    _device->CreateRenderTargetView(backBuffer.Get(), nullptr, _backBuffer.GetAddressOf());
}

void Renderer::SetViewPort()
{
    D3D11_VIEWPORT viewport = {};
    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.Width = _width;
    viewport.Height = _height;
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    _deviceContext->RSSetViewports(1, &viewport);
}
