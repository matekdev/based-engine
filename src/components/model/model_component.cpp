#include "model_component.hpp"

#include "log.hpp"

struct VertexPositionColor
{
    glm::vec3 Position;
    glm::vec3 Color;
};

ModelComponent::ModelComponent(const entt::entity &entity) : Component(entity)
{
}

std::string ModelComponent::GetLoadedModelPath() const
{
    return _loadedModelPath;
}

void ModelComponent::LoadModel(const std::string &modelPath)
{
    if (_loadedModelPath == modelPath)
        return;

    DeleteModel();

    Assimp::Importer import;
    const aiScene *scene = import.ReadFile(modelPath, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        LOG(ERROR) << "Failed to load model: " << import.GetErrorString();
        return;
    }

    _directory = modelPath.substr(0, modelPath.find_last_of('\\'));
    _loadedModelPath = modelPath;

    ProcessNode(scene->mRootNode, scene);
}

void ModelComponent::Render() const
{
    for (const auto &mesh : _meshes)
    {
        mesh.Render();
    }
}

void ModelComponent::DeleteModel()
{
    _meshes.clear();
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
    std::vector<Material> materials;

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

    for (int i = 0; i < mesh->mNumFaces; ++i)
    {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
        {
            indices.push_back(face.mIndices[j]);
        }
    }

    for (int i = 0; i < scene->mNumMaterials; ++i)
    {
        auto &material = *scene->mMaterials[i];
        materials.push_back(Material(material, _directory));
    }

    _hasTextures = !materials.empty();

    return Mesh(vertices, indices, materials);
}
