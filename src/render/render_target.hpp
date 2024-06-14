#pragma once

#include <d3d11.h>
#include <dxgi1_3.h>
#include <wrl.h>

class RenderTarget
{
public:
    RenderTarget();

    ID3D11ShaderResourceView *GetShaderResourceView();
    void Resize();
    void Bind();

private:
    static inline constexpr float _clearColor[] = {0.1f, 0.1f, 0.1f, 1.0f};

    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> _renderTargetView;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> _shaderResourceView;

    void Initialize();
};