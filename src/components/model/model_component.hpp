#pragma once

#include "mesh.hpp"
#include "material.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <entt/entt.hpp>

class ModelComponent
{
public:
    ModelComponent();

    void LoadModel(const std::string &modelPath);
    void Render() const;

private:
    std::string _directory;
    std::vector<Mesh> _meshes;

    void ProcessNode(aiNode *node, const aiScene *scene);
    Mesh ProcessMesh(aiMesh *mesh, const aiScene *scene);
};
