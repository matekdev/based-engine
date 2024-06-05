#pragma once

#include <GLFW/glfw3.h>

#include <d3d11.h>
#include <dxgi1_3.h>
#include <wrl.h>

#include <string>

class DX11Context
{
public:
    DX11Context(GLFWwindow *glfwWindow);
    ~DX11Context();

    const Microsoft::WRL::ComPtr<ID3D11Device> &GetDevice() const;
    const Microsoft::WRL::ComPtr<ID3D11DeviceContext> &GetDeviceContext() const;

    void OnResize(const int &width, const int &height);
    void PreRender() const;
    void PostRender() const;

private:
    int _width;
    int _height;

    Microsoft::WRL::ComPtr<IDXGIFactory2> _dxgiFactory;
    Microsoft::WRL::ComPtr<ID3D11Device> _device;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> _deviceContext;
    Microsoft::WRL::ComPtr<IDXGISwapChain1> _swapChain;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> _renderTargetView;

    void CreateSwapChain();
    void DeleteSwapChain();

    // TODO: Move this stuff into seperate files probably
    Microsoft::WRL::ComPtr<ID3D11InputLayout> _inputLayout;
    Microsoft::WRL::ComPtr<ID3D11VertexShader> _vertexShader = nullptr;
    Microsoft::WRL::ComPtr<ID3D11PixelShader> _pixelShader = nullptr;
    Microsoft::WRL::ComPtr<ID3D11Buffer> _triangleVertices = nullptr;

    Microsoft::WRL::ComPtr<ID3D11VertexShader> CreateVertexShader(const std::wstring &fileName, Microsoft::WRL::ComPtr<ID3DBlob> &vertexShaderBlob) const;
    Microsoft::WRL::ComPtr<ID3D11PixelShader> CreatePixelShader(const std::wstring &fileName) const;
    bool CompileShader(
        const std::wstring &fileName,
        const std::string &entryPoint,
        const std::string &profile,
        Microsoft::WRL::ComPtr<ID3DBlob> &shaderBlob) const;

    void InitializeShaders();
};
