#pragma once

#include <glad/glad.h>

#include <string>

#include "Bindable.hpp"

enum class eTextureType
{
	TEX2D,
	TEX3D,
	HDR,
	SIZE
};

// TODO: Make drawable
struct Texture : Bindable
{
	Texture() = default;
	virtual ~Texture() = default;
	GLuint texture;
	GLuint slot;
    
	void set_texture_slot(GLuint unit)
	{
		slot = unit;
	}
    
	eTextureType type = eTextureType::SIZE;
	virtual void bind(RenderState& gfx) = 0;
	virtual void unbind(RenderState& gfx) = 0;
};
