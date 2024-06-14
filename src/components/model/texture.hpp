#pragma once

#include <string>
#include <vector>

struct Texture
{
    std::string Type;
    std::string Path;
};

unsigned int TextureFromFile(const char *path, const std::string &directory);