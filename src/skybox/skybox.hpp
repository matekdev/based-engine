#pragma once

#include "render/shader.hpp"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <string>

class SkyBox
{
public:
    SkyBox();

    void Render(Shader &shader);

private:
    GLuint _vao;
    GLuint _vbo;
    GLuint _ebo;
    GLuint _textureId;

    // Maybe the skybox should be loaded in as a model via assimp?
    static inline const std::vector<std::string> _imagePaths{
        "models/cubemap/right.jpg",
        "models/cubemap/left.jpg",
        "models/cubemap/top.jpg",
        "models/cubemap/bottom.jpg",
        "models/cubemap/front.jpg",
        "models/cubemap/back.jpg"};

    static inline float _vertices[] =
        {
            -1.0f, -1.0f, 1.0f,
            1.0f, -1.0f, 1.0f,
            1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,
            -1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, -1.0f,
            -1.0f, 1.0f, -1.0f};

    static inline unsigned int _indices[] =
        {
            1, 2, 6,
            6, 5, 1,
            0, 4, 7,
            7, 3, 0,
            4, 5, 6,
            6, 7, 4,
            0, 3, 2,
            2, 1, 0,
            0, 1, 5,
            5, 4, 0,
            3, 7, 6,
            6, 2, 3};
};
