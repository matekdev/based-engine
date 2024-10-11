#pragma once

#include "constant_buffer.hpp"

#include <glm/glm.hpp>

#include <d3d11.h>
#include <dxgi1_3.h>
#include <wrl.h>

class ShadowMap
{
public:
    struct ShadowMapBuffer
    {
        glm::mat4 LightSpaceMatrix;
    };

    ShadowMap();

    void Bind();
    void AssignMap();
    void SetLightSpaceMatrix(const glm::mat4 &matrix);

private:
    ConstantBuffer<ShadowMapBuffer> _constantBuffer;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> _shaderResourceView;
    Microsoft::WRL::ComPtr<ID3D11DepthStencilView> _depthStencilView;
};
