#include "Texture3D.hpp"

#include <cassert>
#include <vector>

Texture3D::Texture3D(int w, int h, int d)
: width(w), height(w), depth(d)
{
    type = eTextureType::TEX3D;
    
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_3D, texture);
    
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    glTexStorage3D(GL_TEXTURE_3D, 8, GL_RGBA8, width, height, depth);
    
    std::vector<GLubyte> data(width*height*depth*4, 0);
    glTexSubImage3D(GL_TEXTURE_3D, 0, 0, 0, 0, width, height, depth, GL_RGBA, GL_UNSIGNED_BYTE, data.data());
    glBindTexture(GL_TEXTURE_3D, 0);
}

Texture3D::~Texture3D()
{
    glDeleteTextures(1, &texture);
}

void Texture3D::clear()
{
    glBindTexture(GL_TEXTURE_3D, texture);
    
    std::vector<GLubyte> data(width*height*depth*4, 0);
    glTexSubImage3D(GL_TEXTURE_3D, 0, 0, 0, 0, width, height, depth, GL_RGBA, GL_UNSIGNED_BYTE, data.data());
    
    glBindTexture(GL_TEXTURE_3D, 0);
}

void Texture3D::generate_mipmaps()
{
    glBindTexture(GL_TEXTURE_3D, texture);
    glGenerateMipmap(GL_TEXTURE_3D);
    glBindTexture(GL_TEXTURE_3D, 0);
}

void Texture3D::bind(RenderState& gfx)
{
    assert(slot >= 0 && slot < 32);
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_3D, texture);
}

void Texture3D::unbind(RenderState& gfx)
{
    assert(slot >= 0 && slot < 32);
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_3D, 0);
}