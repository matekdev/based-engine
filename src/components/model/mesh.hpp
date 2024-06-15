#pragma once

#include "vertex.hpp"

#include <vector>

#include <d3d11.h>
#include <dxgi1_3.h>
#include <wrl.h>

class Mesh
{
public:
    Mesh(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices);

    void Render() const;

private:
    static const uint32_t VERTEX_STRIDE = sizeof(Vertex);
    static const uint32_t VERTEX_OFFSET = 0;

    Microsoft::WRL::ComPtr<ID3D11Buffer> _vertexBuffer;
    Microsoft::WRL::ComPtr<ID3D11Buffer> _indexBuffer;
    uint32_t _indexCount;

    std::vector<Vertex> _vertices;
    std::vector<unsigned int> _indices;

    void InitializeVertexBuffer();
    void InitializeIndexBuffer();
};
