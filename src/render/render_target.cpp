#include "render_target.hpp"

#include "render/renderer.hpp"
#include "ui/scene_panel.hpp"
#include "log.hpp"

RenderTarget::RenderTarget()
{
    Initialize();
    SetViewPort();
}

ID3D11ShaderResourceView *RenderTarget::GetShaderResourceView()
{
    return _shaderResourceView.Get();
}

void RenderTarget::Resize()
{
    Initialize();
    SetViewPort();
}

void RenderTarget::Bind()
{
    Renderer::GetDeviceContext()->OMSetRenderTargets(1, _renderTargetView.GetAddressOf(), _depthStencilView.Get());
    Renderer::GetDeviceContext()->ClearDepthStencilView(_depthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
    Renderer::GetDeviceContext()->ClearRenderTargetView(_renderTargetView.Get(), _clearColor);
}

void RenderTarget::Initialize()
{
    Microsoft::WRL::ComPtr<ID3D11Texture2D> texture2D;

    D3D11_TEXTURE2D_DESC textureDesc = {};
    textureDesc.Width = ScenePanel::GetWidth();
    textureDesc.Height = ScenePanel::GetHeight();
    textureDesc.ArraySize = 1;
    textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    textureDesc.Usage = D3D11_USAGE_DEFAULT;
    textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
    textureDesc.MiscFlags = 0;
    textureDesc.CPUAccessFlags = 0;
    textureDesc.MipLevels = 1;
    textureDesc.SampleDesc.Count = 1;
    textureDesc.SampleDesc.Quality = 0;

    if (FAILED(Renderer::GetDevice()->CreateTexture2D(&textureDesc, nullptr, &texture2D)))
        LOG(ERROR) << "Failed to create texture 2D";

    D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc = {};
    renderTargetViewDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
    renderTargetViewDesc.Texture2D = D3D11_TEX2D_RTV{0};

    if (FAILED(Renderer::GetDevice()->CreateRenderTargetView(texture2D.Get(), &renderTargetViewDesc, &_renderTargetView)))
        LOG(ERROR) << "Failed to create render target view description";

    D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc = {};
    shaderResourceViewDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
    shaderResourceViewDesc.Texture2D.MipLevels = 1;

    if (FAILED(Renderer::GetDevice()->CreateShaderResourceView(texture2D.Get(), &shaderResourceViewDesc, &_shaderResourceView)))
        LOG(ERROR) << "Failed to create shader resource view description";

    Microsoft::WRL::ComPtr<ID3D11Texture2D> depthStencilTexture;

    D3D11_TEXTURE2D_DESC depthStencilTextureDesc = {};
    depthStencilTextureDesc.Width = ScenePanel::GetWidth();
    depthStencilTextureDesc.Height = ScenePanel::GetHeight();
    depthStencilTextureDesc.ArraySize = 1;
    depthStencilTextureDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
    depthStencilTextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
    depthStencilTextureDesc.MiscFlags = 0;
    depthStencilTextureDesc.CPUAccessFlags = 0;
    depthStencilTextureDesc.MipLevels = 1;
    depthStencilTextureDesc.SampleDesc.Count = 1;
    depthStencilTextureDesc.SampleDesc.Quality = 0;

    if (FAILED(Renderer::GetDevice()->CreateTexture2D(&depthStencilTextureDesc, nullptr, &depthStencilTexture)))
        LOG(ERROR) << "Failed to create depth stencil texture";

    D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
    dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    dsvDesc.Texture2D.MipSlice = 0;
    dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

    if (FAILED(Renderer::GetDevice()->CreateDepthStencilView(depthStencilTexture.Get(), &dsvDesc, &_depthStencilView)))
        LOG(ERROR) << "Failed to create depth stencil resource view";
}

void RenderTarget::SetViewPort()
{
    D3D11_VIEWPORT viewport = {};
    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.Width = ScenePanel::GetWidth();
    viewport.Height = ScenePanel::GetHeight();
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    Renderer::GetDeviceContext()->RSSetViewports(1, &viewport);
}