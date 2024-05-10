#pragma once

#include <glad/glad.h>
#include <string>

struct Texture
{
    GLuint Id;
    std::string Type;
    std::string Path;
};

unsigned int TextureFromFile(const char *path, const std::string &directory);