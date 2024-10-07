#include "renderer.hpp"

#include "common/glfw_util.hpp"
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

Renderer::Renderer()
{
    InitializeFactoryAndDevice();
    InitializeSwapChain();
    InitializeImGui();
    InitializeBackBuffer();
    InitializeSampler();
    InitializeRasterizer();
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
    return _device;
}

const Microsoft::WRL::ComPtr<ID3D11DeviceContext> &Renderer::GetDeviceContext()
{
    return _deviceContext;
}

void Renderer::OnResize()
{
    auto windowSize = GLFWUtil::GetWindowSize();
    _deviceContext->Flush();
    _backBuffer.Reset();
    _swapChain->ResizeBuffers(0, windowSize.x, windowSize.y, DXGI_FORMAT::DXGI_FORMAT_B8G8R8A8_UNORM, 0);
    InitializeBackBuffer();
    SetViewPort();
}

void Renderer::BindBackBuffer() const
{
    _deviceContext->OMSetRenderTargets(1, _backBuffer.GetAddressOf(), nullptr);
    _deviceContext->ClearRenderTargetView(_backBuffer.Get(), _clearColor);

    // TODO: This seems like the wrong place to put this.
    _deviceContext->PSSetSamplers(0, 1, _samplerState.GetAddressOf());
    _deviceContext->RSSetState(_rasterizerState.Get());
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
    auto windowSize = GLFWUtil::GetWindowSize();
    DXGI_SWAP_CHAIN_DESC1 swapChainDescriptor = {};
    swapChainDescriptor.Format = DXGI_FORMAT::DXGI_FORMAT_B8G8R8A8_UNORM;
    swapChainDescriptor.Width = windowSize.x;
    swapChainDescriptor.Height = windowSize.y;
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
            glfwGetWin32Window(GLFWUtil::GetNativeWindow()),
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

    ImGui_ImplGlfw_InitForOther(GLFWUtil::GetNativeWindow(), true);
    ImGui_ImplDX11_Init(_device.Get(), _deviceContext.Get());
}

void Renderer::InitializeBackBuffer()
{
    Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;
    _swapChain->GetBuffer(0, IID_PPV_ARGS(backBuffer.GetAddressOf()));
    _device->CreateRenderTargetView(backBuffer.Get(), nullptr, _backBuffer.GetAddressOf());
}

void Renderer::InitializeSampler()
{
    D3D11_SAMPLER_DESC samplerDesc = {};
    samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
    samplerDesc.MipLODBias = 0.0f;
    samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;

    if (FAILED(_device->CreateSamplerState(&samplerDesc, _samplerState.GetAddressOf())))
        LOG(ERROR) << "Failed to create sampler state";
}

void Renderer::InitializeRasterizer()
{
    D3D11_RASTERIZER_DESC rasterizerDesc = {};
    rasterizerDesc.FillMode = D3D11_FILL_SOLID;
    rasterizerDesc.CullMode = D3D11_CULL_NONE;

    if (FAILED(_device->CreateRasterizerState(&rasterizerDesc, _rasterizerState.GetAddressOf())))
        LOG(ERROR) << "Failed to create rasterizer";
}

void Renderer::SetViewPort()
{
    auto windowSize = GLFWUtil::GetWindowSize();
    D3D11_VIEWPORT viewport = {};
    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.Width = windowSize.x;
    viewport.Height = windowSize.y;
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    _deviceContext->RSSetViewports(1, &viewport);
}
