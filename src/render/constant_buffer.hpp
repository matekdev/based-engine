#pragma once

#include "render/renderer.hpp"
#include "log.hpp"

#include <d3d11.h>
#include <wrl.h>

enum class ConstantType
{
    CAMERA_MATRIX,
    MODEL_MATRIX,
    DIRECTIONAL_LIGHT,
    SHADOW_MAP,
};

template <typename T>
class ConstantBuffer
{
public:
    ConstantBuffer(ConstantType type, const T &constant) : _slot(int(type))
    {
        D3D11_BUFFER_DESC bufferDesc = {};
        bufferDesc.ByteWidth = sizeof(T);
        bufferDesc.StructureByteStride = 0;
        bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
        bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        bufferDesc.MiscFlags = 0;

        D3D11_SUBRESOURCE_DATA subresourceData = {};
        subresourceData.pSysMem = &constant;

        if (FAILED(Renderer::GetDevice()->CreateBuffer(&bufferDesc, &subresourceData, &_constantBuffer)))
            LOG(ERROR) << "Failed to create constant buffer";
    }

    void Update(const T &constant)
    {
        D3D11_MAPPED_SUBRESOURCE mappedSubresource = {};
        if (FAILED(Renderer::GetDeviceContext()->Map(_constantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSubresource)))
            LOG(ERROR) << "Failed to update constant buffer";

        memcpy(mappedSubresource.pData, &constant, sizeof(T));
        Renderer::GetDeviceContext()->Unmap(_constantBuffer.Get(), 0);
    }

    void Bind() const
    {
        if (!_constantBuffer)
            return;

        Renderer::GetDeviceContext()->VSSetConstantBuffers(_slot, 1, _constantBuffer.GetAddressOf());
        Renderer::GetDeviceContext()->PSSetConstantBuffers(_slot, 1, _constantBuffer.GetAddressOf());
    }

private:
    int _slot;
    Microsoft::WRL::ComPtr<ID3D11Buffer> _constantBuffer;
};
