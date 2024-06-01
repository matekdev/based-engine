#include "shadow_map.hpp"

#include <limits>
#include <array>

ShadowMap::ShadowMap() : _fbo{0}, _depthMapTexture{0}, _width{0}, _height{0} {}

void ShadowMap::CreateBuffer(float width, float height)
{
    _width = width;
    _height = height;

    glGenFramebuffers(1, &_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, _fbo);

    glGenTextures(1, &_depthMapTexture);
    glBindTexture(GL_TEXTURE_2D, _depthMapTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, _width, _height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _depthMapTexture, 0);

    // We do NOT need a colour buffer, only a depth buffer for shadows.
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    Unbind();
}

ShadowMap::~ShadowMap()
{
    DeleteBuffer();
}

void ShadowMap::DeleteBuffer()
{
    if (!_fbo)
        return;

    glDeleteFramebuffers(1, &_fbo);
    glDeleteTextures(1, &_depthMapTexture);
}

void ShadowMap::Bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
    glViewport(0, 0, _width, _height);
}

void ShadowMap::Unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ShadowMap::BindTexture(Shader &shader)
{
    glActiveTexture(GL_TEXTURE11);
    glBindTexture(GL_TEXTURE_2D, _depthMapTexture);
    shader.SetInt(Shader::SHADOW_MAP_TEXTURE, 11);
}
