#pragma once

#include <d3d11.h>
#include <dxgi1_3.h>
#include <wrl.h>

#include <string>

enum class TextureType
{
    DIFFUSE,
    NORMAL,
    SPECULAR
};

class Texture
{
public:
    Texture(const TextureType &type, const std::string &path);

    void Bind() const;

private:
    uint32_t _slot;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> _shaderResourceView;
};