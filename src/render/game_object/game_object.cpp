#include "game_object.hpp"

#include "log.hpp"
#include "game.hpp"
#include "render/camera.hpp"

#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include <numeric>
#include <algorithm>
#include <iterator>
#include <functional>

GameObject::GameObject(const std::string &filePath, const GameObject::Type &type) : _type(type)
{
    Assimp::Importer import;
    const aiScene *scene = import.ReadFile(filePath, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        LOG(ERROR) << "ERROR::ASSIMP:: " << import.GetErrorString();
        return;
    }

    Name = filePath.substr(filePath.find_last_of("\\") + 1) + std::to_string(_id++);
    _directory = filePath.substr(0, filePath.find_last_of('\\'));

    if (type == GameObject::Type::Light)
    {
        Ambient = glm::vec3(1.0f);
        Diffuse = glm::vec3(1.0f);
    }

    ProcessNode(scene->mRootNode, scene);
    CalculateStats();
}

GameObject::Type GameObject::GetType()
{
    return _type;
}

int GameObject::GetVertexCount()
{
    return _vertexCount;
}

int GameObject::GetTriangleCount()
{
    return _triangleCount;
}

glm::mat4 GameObject::GetTransform()
{
    glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));
    return glm::translate(glm::mat4(1.0f), Position) * rotation * glm::scale(glm::mat4(1.0f), Scale);
}

void GameObject::Render(Shader &shader)
{
    shader.Bind();
    shader.SetVec3(Shader::CAMERA_POSITION, Camera::Instance->GetPosition());
    shader.SetMat4(Shader::CAMERA_MATRIX, Camera::Instance->GetViewProjectionMatrix());
    shader.SetBool(Shader::HAS_TEXTURE, _hasTextures);
    shader.SetMat4(Shader::MODEL_MATRIX, GetTransform());

    shader.SetVec3(Shader::MATERIAL_AMBIENT, Ambient);
    shader.SetVec3(Shader::MATERIAL_DIFFUSE, Diffuse);
    shader.SetVec3(Shader::MATERIAL_SPECULAR, Specular);
    shader.SetFloat(Shader::MATERIAL_SHININESS, Shininess);

    // We need a better way of doing this!! 💩
    std::vector<GameObject *> lights;
    for (auto &gameObject : Game::GameObjects)
        if (gameObject.GetType() == GameObject::Type::Light)
            lights.push_back(&gameObject);

    shader.SetInt("LightCount", lights.size());
    for (int i = 0; i < lights.size(); ++i)
    {
        std::string location = "Lights[" + std::to_string(i) + "]";
        shader.SetVec3(location + ".Position", lights[i]->Position);
        shader.SetVec3(location + ".Ambient", lights[i]->Ambient);
        shader.SetVec3(location + ".Diffuse", lights[i]->Diffuse);
        shader.SetVec3(location + ".Specular", lights[i]->Specular);
        shader.SetFloat(location + ".Attenuation", lights[i]->Attenuation);
    }

    for (auto &mesh : _meshes)
    {
        mesh.SetRenderingMode(RenderingMode);
        mesh.Render(shader);
    }
}

void GameObject::RenderOutline(Shader &shader)
{
    glDisable(GL_DEPTH_TEST);
    auto originalScale = Scale;
    auto outlineScale = originalScale * 1.02f;
    Scale = outlineScale;
    Render(shader);
    Scale = originalScale;
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
}

void GameObject::Dispose()
{
    for (auto &mesh : _meshes)
        mesh.Delete();
}

void GameObject::ProcessNode(aiNode *node, const aiScene *scene)
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

Mesh GameObject::ProcessMesh(aiMesh *mesh, const aiScene *scene)
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
    // we assume a convention for sampler names in the shaders. Each diffuse texture should be named
    // as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER.
    // Same applies to other texture as the following list summarizes:
    // diffuse: texture_diffuseN
    // specular: texture_specularN
    // normal: texture_normalN

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

    _hasTextures = textures.size() > 0;

    // return a mesh object created from the extracted mesh data
    return Mesh(vertices, indices, textures);
}

std::vector<Texture> GameObject::LoadMaterialTextures(aiMaterial *material, aiTextureType type, std::string typeName)
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

void GameObject::CalculateStats()
{
    _vertexCount = std::accumulate(
        _meshes.begin(), _meshes.end(), 0,
        [](int accumulator, Mesh &mesh)
        {
            return accumulator + mesh.VertexCount;
        });

    _triangleCount = std::accumulate(
                         _meshes.begin(), _meshes.end(), 0,
                         [](int accumulator, Mesh &mesh)
                         {
                             return accumulator + mesh.IndexCount;
                         }) /
                     3;
}
