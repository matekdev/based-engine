#pragma once

#include "texture.hpp"

#include <assimp/scene.h>

#include <memory>

class Material
{
public:
    Material(const aiMaterial &material, const std::string &directory);

    void Bind() const;

private:
    std::shared_ptr<Texture> _albedo;
};
