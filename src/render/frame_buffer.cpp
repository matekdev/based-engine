#include "frame_buffer.hpp"

#include <limits>
#include <array>

FrameBuffer::FrameBuffer() : _fbo{0}, _textureId{0}, _depthId{0}, _width{0}, _height{0} {}

void FrameBuffer::CreateBuffer(float width, float height)
{
    _width = width;
    _height = height;

    if (_fbo)
        DeleteBuffer();

    glGenFramebuffers(1, &_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, _fbo);

    glGenTextures(1, &_textureId);
    glBindTexture(GL_TEXTURE_2D, _textureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _textureId, 0);

    glGenTextures(1, &_depthId);
    glBindTexture(GL_TEXTURE_2D, _depthId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _depthId, 0);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    Unbind();
}

FrameBuffer::~FrameBuffer()
{
    DeleteBuffer();
}

void FrameBuffer::DeleteBuffer()
{
    if (!_fbo)
        return;

    glDeleteFramebuffers(1, &_fbo);
    glDeleteTextures(1, &_textureId);
    glDeleteTextures(1, &_depthId);
}

void FrameBuffer::Bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
    glViewport(0, 0, _width, _height);
}

void FrameBuffer::Unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

GLuint FrameBuffer::GetTextureId()
{
    return _textureId;
}

glm::vec3 FrameBuffer::EncodeId(int id)
{
    return glm::vec3(
        static_cast<float>((id & 0x000000FF) >> 0) / 255.0f,
        static_cast<float>((id & 0x0000FF00) >> 8) / 255.0f,
        static_cast<float>((id & 0x00FF0000) >> 16) / 255.0f);
}

int FrameBuffer::DecodePixel(float x, float y)
{
    std::array<unsigned char, 4> byte;
    glReadPixels(x, y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, byte.data());
    auto id = byte[0] + byte[1] * 256 + byte[2] * 256 * 256;
    return id != 0x00ffffff ? id : -1;
}
