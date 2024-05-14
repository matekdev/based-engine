#pragma once

#include "mesh.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <entt/entt.hpp>

class ModelComponent
{
public:
    ModelComponent(const entt::entity &entity);
    ~ModelComponent();
    ModelComponent(const ModelComponent &) = delete;

    void LoadModel(const std::string &modelPath);
    void DeleteModel();
    void Render(Shader &shader);

private:
    static inline std::vector<Texture> _texturesLoaded;
    entt::entity _entity;
    std::vector<Mesh> _meshes;
    std::string _directory;
    bool _hasTextures;

    void ProcessNode(aiNode *node, const aiScene *scene);
    Mesh ProcessMesh(aiMesh *mesh, const aiScene *scene);
    std::vector<Texture> LoadMaterialTextures(aiMaterial *material, aiTextureType type, std::string typeName);
};
