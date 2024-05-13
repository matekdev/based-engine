#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

class FrameBuffer
{
public:
    FrameBuffer();
    ~FrameBuffer();

    void CreateBuffer(float width, float height);
    void DeleteBuffer();
    void Bind();
    void Unbind();
    GLuint GetTextureId();
    glm::vec3 EncodeId(int id);
    uint64_t DecodePixel(float x, float y);

private:
    GLuint _fbo;
    GLuint _textureId;
    GLuint _depthId;
    int _width;
    int _height;
};
