#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

// Some duplicated code between this and frame_buffer!!
class ShadowMap
{
public:
    ShadowMap();
    ~ShadowMap();

    void CreateBuffer(float width, float height);
    void DeleteBuffer();
    void Bind();
    void Unbind();
    GLuint GetDepthTexture();

private:
    GLuint _fbo;
    GLuint _depthMapTexture;
    int _width;
    int _height;
};
