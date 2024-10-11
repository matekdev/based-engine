#include "shadow_map.hpp"

#include "components/model/texture.hpp"
#include "renderer.hpp"
#include "log.hpp"

ShadowMap::ShadowMap() : _constantBuffer(ConstantType::SHADOW_MAP, ShadowMapBuffer{})
{
    Microsoft::WRL::ComPtr<ID3D11Texture2D> depthStencilTexture;

    D3D11_TEXTURE2D_DESC shadowMapDesc = {};
    shadowMapDesc.Width = 4096;
    shadowMapDesc.Height = 4096;
    shadowMapDesc.MipLevels = 1;
    shadowMapDesc.ArraySize = 1;
    shadowMapDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
    shadowMapDesc.SampleDesc.Count = 1;
    shadowMapDesc.Usage = D3D11_USAGE_DEFAULT;
    shadowMapDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;

    if (FAILED(Renderer::GetDevice()->CreateTexture2D(&shadowMapDesc, nullptr, &depthStencilTexture)))
        LOG(ERROR) << "Failed to create depth stencil texture";

    D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc = {};
    shaderResourceViewDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
    shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
    shaderResourceViewDesc.Texture2D.MipLevels = 1;

    if (FAILED(Renderer::GetDevice()->CreateShaderResourceView(depthStencilTexture.Get(), &shaderResourceViewDesc, &_shaderResourceView)))
        LOG(ERROR) << "Failed to create shader resource view description";

    D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
    dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    dsvDesc.Texture2D.MipSlice = 0;
    dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

    if (FAILED(Renderer::GetDevice()->CreateDepthStencilView(depthStencilTexture.Get(), &dsvDesc, &_depthStencilView)))
        LOG(ERROR) << "Failed to create depth stencil resource view";
}

void ShadowMap::Bind()
{
    Renderer::GetDeviceContext()->OMSetRenderTargets(0, nullptr, _depthStencilView.Get());
}

void ShadowMap::AssignMap()
{
    Renderer::GetDeviceContext()->PSSetShaderResources((int)TextureType::SHADOW_MAP, 1, _shaderResourceView.GetAddressOf());
}

void ShadowMap::SetLightSpaceMatrix(const glm::mat4 &matrix)
{
    _constantBuffer.Update(ShadowMapBuffer{matrix});
    _constantBuffer.Bind();
}
