#pragma once

#include <d3d11.h>
#include <wrl.h>

#include <string>

class ShaderUtil
{
public:
    static Microsoft::WRL::ComPtr<ID3DBlob> CompileShader(const std::wstring &fileName, const std::string &entryPoint, const std::string &profile);
    static DXGI_FORMAT GetComponentFormat(D3D_REGISTER_COMPONENT_TYPE type, BYTE mask);
};
