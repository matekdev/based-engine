#include "model_component.hpp"

#include "log.hpp"

// TODO: Remove hello triangle
#include "render/renderer.hpp"

struct VertexPositionColor
{
    glm::vec3 Position;
    glm::vec3 Color;
};

ModelComponent::ModelComponent()
{
    // LoadModel("models\\dev_orange_cube\\dev_orange_cube.obj");

    constexpr VertexPositionColor vertices[] = {
        {glm::vec3{0.0f, 0.5f, 0.0f}, glm::vec3{0.25f, 0.39f, 0.19f}},
        {glm::vec3{0.5f, -0.5f, 0.0f}, glm::vec3{0.44f, 0.75f, 0.35f}},
        {glm::vec3{-0.5f, -0.5f, 0.0f}, glm::vec3{0.38f, 0.55f, 0.20f}},
    };
    D3D11_BUFFER_DESC bufferInfo = {};
    bufferInfo.ByteWidth = sizeof(vertices);
    bufferInfo.Usage = D3D11_USAGE::D3D11_USAGE_IMMUTABLE;
    bufferInfo.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;

    D3D11_SUBRESOURCE_DATA resourceData = {};
    resourceData.pSysMem = vertices;

    if (FAILED(Renderer::GetDevice()->CreateBuffer(
            &bufferInfo,
            &resourceData,
            &_triangleVertices)))
        LOG(ERROR) << "Failed to create vertex buffer";
}

void ModelComponent::LoadModel(const std::string &modelPath)
{
    Assimp::Importer import;
    const aiScene *scene = import.ReadFile(modelPath, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        LOG(ERROR) << "Failed to load model: " << import.GetErrorString();
        return;
    }

    ProcessNode(scene->mRootNode, scene);
}

void ModelComponent::Render() const
{
    // for (const auto &mesh : _meshes)
    // {
    //     mesh.Render();
    // }

    constexpr UINT vertexStride = sizeof(VertexPositionColor);
    constexpr UINT vertexOffset = 0;
    Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, _triangleVertices.GetAddressOf(), &vertexStride, &vertexOffset);
    Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    Renderer::GetDeviceContext()->Draw(3, 0);
}

void ModelComponent::ProcessNode(aiNode *node, const aiScene *scene)
{
    for (int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        _meshes.push_back(ProcessMesh(mesh, scene));
    }

    for (int i = 0; i < node->mNumChildren; i++)
    {
        ProcessNode(node->mChildren[i], scene);
    }
}

Mesh ModelComponent::ProcessMesh(aiMesh *mesh, const aiScene *scene)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        glm::vec3 vector;
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.Position = vector;

        if (mesh->HasNormals())
        {
            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.Normal = vector;
        }

        if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
        {
            glm::vec2 vec;
            // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't
            // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.TexCoords = vec;
        }
        else
        {
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);
        }

        vertices.push_back(vertex);
    }

    for (int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
        {
            indices.push_back(face.mIndices[j]);
        }
    }

    // TODO: Bring back texture loading.

    return Mesh(vertices, indices, textures);
}
