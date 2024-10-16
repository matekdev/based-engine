#pragma once

#include "mesh.hpp"
#include "material.hpp"
#include "components/component.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <entt/entt.hpp>

class ModelComponent : public Component
{
public:
    ModelComponent(const entt::entity &entity);

    std::string GetLoadedModelPath() const;

    void LoadModel(const std::string &modelPath);
    void Render() const;

private:
    std::string _directory;
    std::vector<Mesh> _meshes;
    std::string _loadedModelPath;
    bool _hasTextures;

    glm::vec3 _minPoint;
    glm::vec3 _maxPoint;

    void DeleteModel();
    void ProcessNode(aiNode *node, const aiScene *scene);
    Mesh ProcessMesh(aiMesh *mesh, const aiScene *scene);
};
