#include "mesh.hpp"

#include "render/renderer.hpp"
#include "log.hpp"

Mesh::Mesh(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices, const std::vector<Texture> &textures)
    : _vertices(vertices), _indices(indices), _textures(textures), _indexCount((uint32_t)indices.size())
{
    InitializeVertexBuffer();
    InitializeIndexBuffer();
}

void Mesh::Render() const
{
    Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, _vertexBuffer.GetAddressOf(), &VERTEX_STRIDE, &VERTEX_OFFSET);
    Renderer::GetDeviceContext()->IASetIndexBuffer(_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
    Renderer::GetDeviceContext()->DrawIndexed(_indexCount, 0, 0);
}

void Mesh::InitializeVertexBuffer()
{
    D3D11_BUFFER_DESC bufferDesc = {};
    bufferDesc.ByteWidth = (uint32_t)_vertices.size() * VERTEX_STRIDE;
    bufferDesc.StructureByteStride = VERTEX_STRIDE;
    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

    D3D11_SUBRESOURCE_DATA subresourceData = {};
    subresourceData.pSysMem = _vertices.data();

    if (FAILED(Renderer::GetDevice()->CreateBuffer(&bufferDesc, &subresourceData, &_vertexBuffer)))
        LOG(ERROR) << "Failed to create vertex buffer";
}

void Mesh::InitializeIndexBuffer()
{
    D3D11_BUFFER_DESC bufferDesc = {};
    bufferDesc.ByteWidth = (uint32_t)_indices.size() * sizeof(unsigned int);
    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

    D3D11_SUBRESOURCE_DATA subresourceData = {};
    subresourceData.pSysMem = _indices.data();

    if (FAILED(Renderer::GetDevice()->CreateBuffer(&bufferDesc, &subresourceData, &_indexBuffer)))
        LOG(ERROR) << "Failed to create index buffer";
}
