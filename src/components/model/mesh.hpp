#pragma once

#include "vertex.hpp"
#include "material.hpp"

#include <vector>
#include <memory>

#include <d3d11.h>
#include <dxgi1_3.h>
#include <wrl.h>

class Mesh
{
public:
    Mesh(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices, const std::vector<Material> &materials);

    void Render() const;

private:
    static const unsigned int VERTEX_STRIDE = sizeof(Vertex);
    static const unsigned int VERTEX_OFFSET = 0;

    Microsoft::WRL::ComPtr<ID3D11Buffer> _vertexBuffer;
    Microsoft::WRL::ComPtr<ID3D11Buffer> _indexBuffer;
    unsigned int _indexCount;

    std::vector<Vertex> _vertices;
    std::vector<unsigned int> _indices;
    std::vector<Material> _materials;

    void InitializeVertexBuffer();
    void InitializeIndexBuffer();
};
