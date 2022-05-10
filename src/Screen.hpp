#pragma once
#include <array>

#include "glad/glad.h"

#include "Bindable.hpp"

struct Screen : Drawable
{
    Screen(
           std::array<float, 24> quadVertices = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
               // positions   // texCoords
               -1.0f,  1.0f,  0.0f, 1.0f,
               -1.0f, -1.0f,  0.0f, 0.0f,
               1.0f, -1.0f,  1.0f, 0.0f,
               
               -1.0f,  1.0f,  0.0f, 1.0f,
               1.0f, -1.0f,  1.0f, 0.0f,
               1.0f,  1.0f,  1.0f, 1.0f
           }
           )
    {
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
        glBindVertexArray(0);
    }
    
    virtual ~Screen() {
        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &vbo);
    }
    
    void bind(RenderState& gfx) override
    {
        glBindVertexArray(vao);
    }
    void unbind(RenderState& gfx) override
    {
        glBindVertexArray(0);
    }
    void draw(RenderState& gfx) override
    {
        // will bind and unbind the vao
        
        bind(gfx);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        unbind(gfx);
    }
    
    GLuint vao;
    GLuint vbo;
};