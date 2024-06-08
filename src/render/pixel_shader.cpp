#include "pixel_shader.hpp"

#include "shader_util.hpp"
#include "log.hpp"

#include <d3dcompiler.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

PixelShader::PixelShader(const Microsoft::WRL::ComPtr<ID3D11Device> &device, const std::wstring &fileName)
{
    const auto pixelShaderBlob = ShaderUtil::CompileShader(fileName, "Main", "ps_5_0");

    if (FAILED(device->CreatePixelShader(
            pixelShaderBlob->GetBufferPointer(),
            pixelShaderBlob->GetBufferSize(),
            nullptr,
            &_pixelShader)))
        LOG(ERROR) << "Failed to compile pixel shader";
}

void PixelShader::Bind(const Microsoft::WRL::ComPtr<ID3D11DeviceContext> &device) const
{
    device->PSSetShader(_pixelShader.Get(), nullptr, 0);
}
