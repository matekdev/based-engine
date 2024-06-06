#include "shader.hpp"

#include "log.hpp"

#include <d3dcompiler.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

VertexShaderData Shader::CreateVertexShader(const Microsoft::WRL::ComPtr<ID3D11Device> &device, const std::wstring &fileName)
{
    const auto vertexShaderBlob = CompileShader(fileName, "Main", "vs_5_0");

    Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader;
    if (FAILED(device->CreateVertexShader(
            vertexShaderBlob->GetBufferPointer(),
            vertexShaderBlob->GetBufferSize(),
            nullptr,
            &vertexShader)))
        LOG(ERROR) << "Failed to compile vertex shader";

    return VertexShaderData{vertexShader, vertexShaderBlob};
}

Microsoft::WRL::ComPtr<ID3D11PixelShader> Shader::CreatePixelShader(const Microsoft::WRL::ComPtr<ID3D11Device> &device, const std::wstring &fileName)
{
    const auto pixelShaderBlob = CompileShader(fileName, "Main", "ps_5_0");

    Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader;
    if (FAILED(device->CreatePixelShader(
            pixelShaderBlob->GetBufferPointer(),
            pixelShaderBlob->GetBufferSize(),
            nullptr,
            &pixelShader)))
        LOG(ERROR) << "Failed to compile pixel shader";

    return pixelShader;
}

Microsoft::WRL::ComPtr<ID3DBlob> Shader::CompileShader(const std::wstring &fileName, const std::string &entryPoint, const std::string &profile)
{
    const auto compileFlags = D3DCOMPILE_ENABLE_STRICTNESS;

    Microsoft::WRL::ComPtr<ID3DBlob> shaderBlob;
    Microsoft::WRL::ComPtr<ID3DBlob> errorBlob;
    if (FAILED(D3DCompileFromFile(
            fileName.data(),
            nullptr,
            D3D_COMPILE_STANDARD_FILE_INCLUDE,
            entryPoint.data(),
            profile.data(),
            compileFlags,
            0,
            &shaderBlob,
            &errorBlob)))
    {
        if (errorBlob)
        {
            char *compilerErrors = static_cast<char *>(errorBlob->GetBufferPointer());
            LOG(ERROR) << "Failed to compile shaders: " + std::string(compilerErrors);
        }
        return nullptr;
    }

    return shaderBlob;
}
