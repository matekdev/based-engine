#pragma once

#include "components/model/texture.hpp"
#include "render/vertex_shader.hpp"
#include "render/pixel_shader.hpp"

#include <d3d11.h>
#include <dxgi1_3.h>
#include <wrl.h>

#include <vector>
#include <string>

class SkyBox
{
public:
    SkyBox();

    void Render() const;

private:
    VertexShader _skyboxVertexShader;
    PixelShader _skyboxPixelShader;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> _shaderResourceView;

    static inline const std::vector<std::string> _imagePaths{
        "models/skybox/right.png",
        "models/skybox/left.png",
        "models/skybox/down.png",
        "models/skybox/up.png",
        "models/skybox/front.png",
        "models/skybox/back.png"};

    static inline float _vertices[] =
        {
            -1.0f, -1.0f, 1.0f,
            1.0f, -1.0f, 1.0f,
            1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,
            -1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, -1.0f,
            -1.0f, 1.0f, -1.0f};

    static inline unsigned int _indices[] =
        {
            1, 2, 6,
            6, 5, 1,
            0, 4, 7,
            7, 3, 0,
            4, 5, 6,
            6, 7, 4,
            0, 3, 2,
            2, 1, 0,
            0, 1, 5,
            5, 4, 0,
            3, 7, 6,
            6, 2, 3};
};