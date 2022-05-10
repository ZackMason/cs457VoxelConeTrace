#pragma once

#include "Texture.hpp"

struct Texture3D : Texture
{
    Texture3D(int w, int h, int d);
    virtual ~Texture3D();
    
    void bind(RenderState& gfx);
    void unbind(RenderState& gfx);
    
    void generate_mipmaps();
    void clear();
    
    int width, height, depth;
};