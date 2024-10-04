#include "texture.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.hpp"

#include "render/renderer.hpp"
#include "log.hpp"

Texture::Texture(const TextureType &type, const std::string &path) : _slot((unsigned int)type)
{
    int width, height, nrComponents;
    auto data = stbi_load(path.c_str(), &width, &height, &nrComponents, 4);
    if (!data)
    {
        LOG(ERROR) << "Failed to load texture: " << path;
        return;
    }

    Microsoft::WRL::ComPtr<ID3D11Texture2D> texture;

    D3D11_TEXTURE2D_DESC textureDesc = {};
    textureDesc.Width = width;
    textureDesc.Height = height;
    textureDesc.MipLevels = 1;
    textureDesc.ArraySize = 1;
    textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    textureDesc.SampleDesc.Count = 1;
    textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

    D3D11_SUBRESOURCE_DATA subresourceData = {};
    subresourceData.pSysMem = data;
    subresourceData.SysMemPitch = width * sizeof(UINT);

    if (FAILED(Renderer::GetDevice()->CreateTexture2D(&textureDesc, &subresourceData, &texture)))
    {
        LOG(ERROR) << "Failed to create texture: " << path;
        return;
    }

    stbi_image_free(data);

    D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc = {};
    viewDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    viewDesc.Texture2D.MipLevels = 1;

    if (FAILED(Renderer::GetDevice()->CreateShaderResourceView(texture.Get(), &viewDesc, &_shaderResourceView)))
    {
        LOG(ERROR) << "Failed to create shader texture resource: " << path;
        return;
    }
}

void Texture::Bind() const
{
    Renderer::GetDeviceContext()->PSSetShaderResources(_slot, 1, _shaderResourceView.GetAddressOf());
}
