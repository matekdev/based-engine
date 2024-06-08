#pragma once

#include <d3d11.h>
#include <wrl.h>

#include <string>

class VertexShader
{
public:
    VertexShader(Microsoft::WRL::ComPtr<ID3D11Device> &device, const std::wstring &fileName);

    void Bind(const Microsoft::WRL::ComPtr<ID3D11DeviceContext> &device) const;

private:
    Microsoft::WRL::ComPtr<ID3D11VertexShader> _vertexShader;
    Microsoft::WRL::ComPtr<ID3D11InputLayout> _inputLayout;
};