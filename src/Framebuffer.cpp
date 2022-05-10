#include "Framebuffer.hpp"


#include "RenderState.hpp"


Framebuffer::Framebuffer(int w, int h, bool aa)
: width(w), height(h), msaa(aa)
{
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    
	glGenTextures(1, &tbo);
	
	if (msaa)
	{
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, tbo);
		glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGBA16F, width, height, GL_TRUE);
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
	}
	else
	{
		glBindTexture(GL_TEXTURE_2D, tbo);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
	}
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glGenerateMipmap(GL_TEXTURE_2D);
	if (msaa)
	{
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, tbo, 0);
	}
	else
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tbo, 0);
    
    
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	if(msaa)
		glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH24_STENCIL8, width, height);
	else
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
    
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR: FAILED TO COMPLETE FRAMEBUFFER\n";
	else
		std::cout << "Framebuffer created\n";
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


void Framebuffer::blit(Framebuffer& msaa_fbo)
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, msaa_fbo.fbo);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
	glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

Framebuffer::~Framebuffer()
{
	glDeleteFramebuffers(1, &fbo);
	glDeleteRenderbuffers(1, &rbo);
	glDeleteTextures(1, &tbo);
}


void Framebuffer::bind(RenderState& gfx)
{
	switch (mode)
	{
        case eFramebufferMode::RENDER_TARGET:
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        std::cout << "fix frame buffer!!!!!" << std::endl;
		//gfx.ClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		//gfx.Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		break;
        case eFramebufferMode::TEXTURE:
		assert(slot >= 0 && slot < 32);
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, tbo);
		break;
	}
}

void Framebuffer::unbind(RenderState& gfx)
{
	switch (mode)
	{
        case eFramebufferMode::RENDER_TARGET:
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		break;
        case eFramebufferMode::TEXTURE:
		glBindTexture(msaa ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D, 0);
		break;
	}
}

void Framebuffer::clear(RenderState& gfx, const v4f& color, GLenum flags)
{
	//gfx.ClearColor(color.x,color.y, color.z, color.w);
	//gfx.Clear(flags);
}

void Framebuffer::resize(u32 w, u32 h)
{
    
}
