#pragma once

#include <GLFW/glfw3.h>

#include <d3d11.h>
#include <dxgi1_3.h>
#include <wrl.h>

#include <string>
#include <memory>

class Renderer
{
public:
    Renderer();
    ~Renderer();

    static const Microsoft::WRL::ComPtr<ID3D11Device> &GetDevice();
    static const Microsoft::WRL::ComPtr<ID3D11DeviceContext> &GetDeviceContext();

    void OnResize();
    void BindBackBuffer() const;
    void PreRender() const;
    void PostRender() const;

private:
    static inline constexpr float _clearColor[] = {0.1f, 0.1f, 0.1f, 1.0f};

    Microsoft::WRL::ComPtr<IDXGIFactory2> _dxgiFactory;
    static inline Microsoft::WRL::ComPtr<ID3D11Device> _device;
    static inline Microsoft::WRL::ComPtr<ID3D11DeviceContext> _deviceContext;
    Microsoft::WRL::ComPtr<IDXGISwapChain1> _swapChain;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> _backBuffer;
    Microsoft::WRL::ComPtr<ID3D11SamplerState> _samplerState;
    Microsoft::WRL::ComPtr<ID3D11RasterizerState> _rasterizerState;

    void InitializeFactoryAndDevice();
    void InitializeSwapChain();
    void InitializeImGui();
    void InitializeBackBuffer();
    void InitializeSampler();
    void InitializeRasterizer();
    void SetViewPort();
};
