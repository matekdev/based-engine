#include "material.hpp"

#include "log.hpp"

Material::Material(const aiMaterial &material, const std::string &directory)
{
    aiString texturePath;

    if (material.GetTexture(aiTextureType_DIFFUSE, 0, &texturePath) == aiReturn_SUCCESS)
        _albedo = std::make_shared<Texture>(TextureType::DIFFUSE, directory + "\\" + texturePath.C_Str());
}

void Material::Bind() const
{
    if (_albedo)
        _albedo->Bind();
}
