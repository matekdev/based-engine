#pragma once

#include "mesh.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <entt/entt.hpp>

class ModelComponent
{
public:
    glm::vec3 Ambient = glm::vec3(1.0f, 1.0f, 1.0f);
    glm::vec3 Diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
    glm::vec3 Specular = glm::vec3(1.0f, 1.0f, 1.0f);
    float Shininess = 32.0f;

    ModelComponent(const entt::entity &entity);
    ~ModelComponent();
    ModelComponent(const ModelComponent &) = delete;

    std::string GetLoadedModel();

    void LoadModel(const std::string &modelPath);
    void DeleteModel();
    void Render();
    bool HasTextures();

private:
    static inline std::vector<Texture> _texturesLoaded;

    entt::entity _entity;
    std::string _loadedModel;

    std::vector<Mesh> _meshes;
    std::string _directory;
    bool _hasTextures;

    void ProcessNode(aiNode *node, const aiScene *scene);
    Mesh ProcessMesh(aiMesh *mesh, const aiScene *scene);
    std::vector<Texture> LoadMaterialTextures(aiMaterial *material, aiTextureType type, std::string typeName);
};
