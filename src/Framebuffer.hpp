#pragma once
#include <glad/glad.h>

#include "Bindable.hpp"
#include "Core.hpp"


enum class eFramebufferMode
{
	RENDER_TARGET, TEXTURE, SIZE
};

struct Framebuffer : Bindable
{
	Framebuffer(int width, int height, bool _msaa = false);
	~Framebuffer();
    
	void bind(RenderState& gfx) override;
	void unbind(RenderState& gfx) override;
	void clear(RenderState& gfx, const v4f& color = { 0.0, 0.0, 0.0, 0.0 }, GLenum flags = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
	void resize(u32 w, u32 h);
	void blit(Framebuffer& msaa_fbo);
    
	eFramebufferMode mode = eFramebufferMode::RENDER_TARGET;
    
	u32 width, height;
    
	bool msaa;
    
	GLuint tbo;
	GLuint rbo;
	GLuint fbo;
	GLuint slot;
	//bool resize = true;
};

