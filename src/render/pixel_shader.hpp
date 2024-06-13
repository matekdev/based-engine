#pragma once

#include <d3d11.h>
#include <wrl.h>

#include <string>

class PixelShader
{
public:
    PixelShader(const std::wstring &fileName);

    void Bind() const;

private:
    Microsoft::WRL::ComPtr<ID3D11PixelShader> _pixelShader;
};
