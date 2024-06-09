#include "texture.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.hpp"

#include "log.hpp"

#include <d3d11.h>
#include <d3dcompiler.h>

unsigned int TextureFromFile(const char *path, const std::string &directory)
{
    std::string filename = std::string(path);
    filename = directory + '\\' + filename;

    int width, height, nrComponents;
    unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    if (data)
    {
        DXGI_FORMAT format;
        if (nrComponents == 1)
            format = DXGI_FORMAT_R8_UNORM;
        else if (nrComponents == 3)
            format = DXGI_FORMAT_R8G8B8A8_UNORM;
        else if (nrComponents == 4)
            format = DXGI_FORMAT_R8G8B8A8_UNORM;

        stbi_image_free(data);
    }
    else
    {
        LOG(ERROR) << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return -1;
}
