#include "vertex_shader.hpp"

#include "shader_util.hpp"
#include "log.hpp"

#include <d3dcompiler.h>
#include <d3d11shader.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")

VertexShader::VertexShader(Microsoft::WRL::ComPtr<ID3D11Device> &device, const std::wstring &fileName)
{
    const auto vertexShaderBlob = ShaderUtil::CompileShader(fileName, "Main", "vs_5_0");

    if (FAILED(device->CreateVertexShader(
            vertexShaderBlob->GetBufferPointer(),
            vertexShaderBlob->GetBufferSize(),
            nullptr,
            &_vertexShader)))
        LOG(ERROR) << "Failed to compile vertex shader";

    Microsoft::WRL::ComPtr<ID3D11ShaderReflection> shaderRefleection;
    D3D11_SHADER_DESC shaderDesc = {};
    std::vector<D3D11_INPUT_ELEMENT_DESC> inputElementDescs;

    D3DReflect(vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize(), IID_ID3D11ShaderReflection, &shaderRefleection);
    shaderRefleection->GetDesc(&shaderDesc);

    for (int i = 0; i < shaderDesc.InputParameters; i++)
    {
        D3D11_SIGNATURE_PARAMETER_DESC spDesc = {};
        shaderRefleection->GetInputParameterDesc(i, &spDesc);

        D3D11_INPUT_ELEMENT_DESC desc = {};
        desc.SemanticName = spDesc.SemanticName;
        desc.SemanticIndex = spDesc.SemanticIndex;
        desc.InputSlot = 0;
        desc.Format = ShaderUtil::GetComponentFormat(spDesc.ComponentType, spDesc.Mask);
        desc.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
        desc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
        desc.InstanceDataStepRate = 0;

        inputElementDescs.push_back(std::move(desc));
    }

    device->CreateInputLayout(inputElementDescs.data(),
                              (UINT)inputElementDescs.size(),
                              vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize(),
                              &_inputLayout);
}

void VertexShader::Bind(const Microsoft::WRL::ComPtr<ID3D11DeviceContext> &device) const
{
    device->IASetInputLayout(_inputLayout.Get());
    device->VSSetShader(_vertexShader.Get(), nullptr, 0);
}
