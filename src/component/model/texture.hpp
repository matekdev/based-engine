#pragma once

#include <glad/glad.h>
#include <string>
#include <vector>

struct Texture
{
    GLuint Id;
    std::string Type;
    std::string Path;
};

unsigned int TextureFromFile(const char *path, const std::string &directory);
unsigned int LoadCubeMap(const std::vector<std::string> &paths);