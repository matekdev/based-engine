#include "skybox.hpp"

#include "render/stb_image.hpp"

#include "log.hpp"
#include "renderer.hpp"

SkyBox::SkyBox() : _skyboxVertexShader(L"shaders/skybox.vs.hlsl"),
                   _skyboxPixelShader(L"shaders/skybox.ps.hlsl")
{
    const int size = 6;
    int width, height, nrComponents;
    std::vector<stbi_uc *> textureData(size);
    for (int i = 0; i < size; ++i)
    {
        textureData[i] = stbi_load(_imagePaths[i].c_str(), &width, &height, &nrComponents, 4);
    }

    D3D11_TEXTURE2D_DESC textureDesc = {};
    textureDesc.Width = width;
    textureDesc.Height = height;
    textureDesc.MipLevels = 1;
    textureDesc.ArraySize = size;
    textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    textureDesc.SampleDesc.Count = 1;
    textureDesc.SampleDesc.Quality = 0;
    textureDesc.Usage = D3D11_USAGE_DEFAULT;
    textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    textureDesc.CPUAccessFlags = 0;
    textureDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;

    D3D11_SUBRESOURCE_DATA data[size];
    for (int i = 0; i < size; i++)
    {
        data[i].pSysMem = textureData[i];
        data[i].SysMemPitch = width * sizeof(UINT);
        data[i].SysMemSlicePitch = 0;
    }

    Microsoft::WRL::ComPtr<ID3D11Texture2D> texture;
    Renderer::GetDevice()->CreateTexture2D(&textureDesc, data, &texture);

    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
    srvDesc.Format = textureDesc.Format;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
    srvDesc.TextureCube.MostDetailedMip = 0;
    srvDesc.TextureCube.MipLevels = 1;

    Renderer::GetDevice()
        ->CreateShaderResourceView(texture.Get(), &srvDesc, &_shaderResourceView);

    for (int i = 0; i < size; ++i)
    {
        stbi_image_free(textureData[i]);
    }
}

void SkyBox::Render() const
{
    Renderer::GetDeviceContext()->PSSetShaderResources((unsigned int)TextureType::SKYBOX, 1, _shaderResourceView.GetAddressOf());
}
