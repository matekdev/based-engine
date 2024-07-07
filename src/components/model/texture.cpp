#include "texture.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.hpp"

#include "render/renderer.hpp"
#include "log.hpp"

Texture::Texture(const TextureType &type, const std::string &path) : _slot((uint32_t)type)
{
    int width, height, nrComponents;
    auto data = stbi_load(path.c_str(), &width, &height, &nrComponents, 0);
    if (!data)
    {
        LOG(ERROR) << "Failed to load texture: " << path;
        stbi_image_free(data);
        return;
    }

    Microsoft::WRL::ComPtr<ID3D11Texture2D> texture;

    D3D11_TEXTURE2D_DESC textureDesc = {};
    textureDesc.Width = width;
    textureDesc.Height = height;
    textureDesc.ArraySize = 1;
    textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
    textureDesc.Usage = D3D11_USAGE_DEFAULT;
    textureDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;
    textureDesc.CPUAccessFlags = 0;
    textureDesc.MipLevels = 0;
    textureDesc.SampleDesc.Count = 1;
    textureDesc.SampleDesc.Quality = 0;

    if (FAILED(Renderer::GetDevice()->CreateTexture2D(&textureDesc, nullptr, &texture)))
    {
        LOG(ERROR) << "Failed to create texture: " << path;
        return;
    }

    Renderer::GetDeviceContext()->UpdateSubresource(texture.Get(), 0, nullptr, data, width * sizeof(UINT), 0);

    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
    srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MostDetailedMip = 0;
    srvDesc.Texture2D.MipLevels = -1;

    Renderer::GetDevice()->CreateShaderResourceView(texture.Get(), &srvDesc, &_shaderResourceView);
    Renderer::GetDeviceContext()->GenerateMips(_shaderResourceView.Get());
}

void Texture::Bind() const
{
    Renderer::GetDeviceContext()->PSSetShaderResources(_slot, 1, _shaderResourceView.GetAddressOf());
}
