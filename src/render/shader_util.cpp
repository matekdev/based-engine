#include "shader_util.hpp"

#include "log.hpp"

#include <d3dcompiler.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

Microsoft::WRL::ComPtr<ID3DBlob> ShaderUtil::CompileShader(const std::wstring &fileName, const std::string &entryPoint, const std::string &profile)
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

DXGI_FORMAT ShaderUtil::GetComponentFormat(D3D_REGISTER_COMPONENT_TYPE type, BYTE mask)
{
    if (mask == 1)
    {
        if (type == D3D_REGISTER_COMPONENT_UINT32)
            return DXGI_FORMAT_R32_UINT;
        else if (type == D3D_REGISTER_COMPONENT_SINT32)
            return DXGI_FORMAT_R32_SINT;
        else if (type == D3D_REGISTER_COMPONENT_FLOAT32)
            return DXGI_FORMAT_R32_FLOAT;
    }
    else if (mask <= 3)
    {
        if (type == D3D_REGISTER_COMPONENT_UINT32)
            return DXGI_FORMAT_R32G32_UINT;
        else if (type == D3D_REGISTER_COMPONENT_SINT32)
            return DXGI_FORMAT_R32G32_SINT;
        else if (type == D3D_REGISTER_COMPONENT_FLOAT32)
            return DXGI_FORMAT_R32G32_FLOAT;
    }
    else if (mask <= 7)
    {
        if (type == D3D_REGISTER_COMPONENT_UINT32)
            return DXGI_FORMAT_R32G32B32_UINT;
        else if (type == D3D_REGISTER_COMPONENT_SINT32)
            return DXGI_FORMAT_R32G32B32_SINT;
        else if (type == D3D_REGISTER_COMPONENT_FLOAT32)
            return DXGI_FORMAT_R32G32B32_FLOAT;
    }
    else if (mask <= 15)
    {
        if (type == D3D_REGISTER_COMPONENT_UINT32)
            return DXGI_FORMAT_R32G32B32A32_UINT;
        else if (type == D3D_REGISTER_COMPONENT_SINT32)
            return DXGI_FORMAT_R32G32B32A32_SINT;
        else if (type == D3D_REGISTER_COMPONENT_FLOAT32)
            return DXGI_FORMAT_R32G32B32A32_FLOAT;
    }
    return DXGI_FORMAT_UNKNOWN;
}
