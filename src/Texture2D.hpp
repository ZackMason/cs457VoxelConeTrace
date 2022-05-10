#pragma once

#include "Texture.hpp"

struct Texture2D : Texture
{
	Texture2D() = default;
	Texture2D(int width, int height);
	Texture2D(const std::string&);
	virtual ~Texture2D();
    
	void set_data(void* data, uint32_t size);
	void bind(RenderState& gfx) override;
	void unbind(RenderState& gfx) override;
    
	int width;
	int height;
	int channels;
	GLenum internal_format, data_format;
};
