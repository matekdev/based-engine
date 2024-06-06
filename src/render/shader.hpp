#pragma once

#include <d3d11.h>
#include <wrl.h>

#include <string>
#include <tuple>

struct VertexShaderData
{
    Microsoft::WRL::ComPtr<ID3D11VertexShader> VertexShader;
    Microsoft::WRL::ComPtr<ID3DBlob> VertexShaderBlob;
};

class Shader
{
public:
    static VertexShaderData CreateVertexShader(const Microsoft::WRL::ComPtr<ID3D11Device> &device, const std::wstring &fileName);
    static Microsoft::WRL::ComPtr<ID3D11PixelShader> CreatePixelShader(const Microsoft::WRL::ComPtr<ID3D11Device> &device, const std::wstring &fileName);

private:
    static Microsoft::WRL::ComPtr<ID3DBlob> CompileShader(const std::wstring &fileName, const std::string &entryPoint, const std::string &profile);
};
