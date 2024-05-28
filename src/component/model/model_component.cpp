#include "component/model/model_component.hpp"

#include "scene.hpp"
#include "camera/camera.hpp"
#include "component/transform_component.hpp"
#include "log.hpp"

#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include <numeric>
#include <algorithm>
#include <iterator>
#include <functional>

ModelComponent::ModelComponent(const entt::entity &entity) : _entity(entity)
{
}

ModelComponent::~ModelComponent()
{
    DeleteModel();
}

std::string ModelComponent::GetLoadedModel()
{
    return _loadedModel;
}

void ModelComponent::LoadModel(const std::string &modelPath)
{
    Assimp::Importer import;
    const aiScene *scene = import.ReadFile(modelPath, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        LOG(ERROR) << "ERROR::ASSIMP:: " << import.GetErrorString();
        return;
    }

    _loadedModel = modelPath;
    _directory = modelPath.substr(0, modelPath.find_last_of('\\'));

    ProcessNode(scene->mRootNode, scene);
}

void ModelComponent::DeleteModel()
{
    for (auto &mesh : _meshes)
    {
        mesh.Delete();
    }

    _meshes.clear();
    _loadedModel = "";
}

void ModelComponent::Render(Shader &shader)
{
    for (auto &mesh : _meshes)
    {
        mesh.Render(shader);
    }
}

bool ModelComponent::HasTextures()
{
    return _hasTextures;
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

    aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

    // 1. diffuse maps
    std::vector<Texture> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
    // 2. specular maps
    std::vector<Texture> specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    // 3. normal maps
    std::vector<Texture> normalMaps = LoadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
    textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
    // 4. height maps
    std::vector<Texture> heightMaps = LoadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
    textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

    _hasTextures = !textures.empty();

    return Mesh(vertices, indices, textures);
}

std::vector<Texture> ModelComponent::LoadMaterialTextures(aiMaterial *material, aiTextureType type, std::string typeName)
{
    std::vector<Texture> textures;
    for (int i = 0; i < material->GetTextureCount(type); i++)
    {
        aiString str;
        material->GetTexture(type, i, &str);
        bool skipLoading = false;
        for (unsigned int j = 0; j < _texturesLoaded.size(); j++)
        {
            if (std::strcmp(_texturesLoaded[j].Path.data(), str.C_Str()) == 0)
            {
                textures.push_back(_texturesLoaded[j]);
                skipLoading = true;
                break;
            }
        }

        if (!skipLoading)
        {
            Texture texture;
            texture.Id = TextureFromFile(str.C_Str(), _directory);
            texture.Type = typeName;
            texture.Path = str.C_Str();
            textures.push_back(texture);
            _texturesLoaded.push_back(texture);
        }
    }

    return textures;
}