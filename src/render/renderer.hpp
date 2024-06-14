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

    static void OnResize();
    void BindBackBuffer() const;
    void PreRender() const;
    void PostRender() const;

private:
    static inline Renderer *_instance;
    static inline constexpr float _clearColor[] = {0.1f, 0.1f, 0.1f, 1.0f};

    Microsoft::WRL::ComPtr<IDXGIFactory2> _dxgiFactory;
    Microsoft::WRL::ComPtr<ID3D11Device> _device;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> _deviceContext;
    Microsoft::WRL::ComPtr<IDXGISwapChain1> _swapChain;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> _backBuffer;

    void InitializeFactoryAndDevice();
    void InitializeSwapChain();
    void InitializeImGui();
    void InitializeBackBuffer();
    void SetViewPort();
};
