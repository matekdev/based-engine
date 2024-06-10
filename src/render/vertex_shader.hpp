#pragma once

#include <d3d11.h>
#include <wrl.h>

#include <string>

class VertexShader
{
public:
    VertexShader(const std::wstring &fileName);

    void Bind() const;

private:
    Microsoft::WRL::ComPtr<ID3D11VertexShader> _vertexShader;
    Microsoft::WRL::ComPtr<ID3D11InputLayout> _inputLayout;
};