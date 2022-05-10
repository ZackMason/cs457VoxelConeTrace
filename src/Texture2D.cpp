#include "Texture2D.hpp"

#include <string>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture2D::Texture2D(const std::string& file)
{
	internal_format = GL_RGBA8;
	data_format = GL_RGBA;
    
	stbi_set_flip_vertically_on_load(true);
    
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	float aniso = 16.;
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &aniso);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, aniso);
	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load and generate the texture
    
	const std::string path = file;
    
	unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
	if (data)
	{
		if (channels == 3)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		else if (channels == 4)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		std::cout << "LOADED TEXTURE: " << file << "\n";
	}
	else
	{
		data = stbi_load("./assets/UVgrid.png", &width, &height, &channels, 0);
		if (data)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
		std::cout << "Failed to load texture "<<  file <<"\n";
	}
	stbi_image_free(data);
}

Texture2D::Texture2D(const int w, const int h)
: width(w), height(h)
{
	internal_format = GL_RGBA8;
	data_format = GL_RGBA;
    
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
    
	//glCreateTextures(GL_TEXTURE_2D, 1, &m_Texture);
	glTexStorage2D(GL_TEXTURE_2D, 1, internal_format, width, height);
    
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, height, 0, GL_RGBA8, GL_UNSIGNED_BYTE, nullptr);
	//glGenerateMipmap(GL_TEXTURE_2D);
}

void Texture2D::set_data(void* data, uint32_t size)
{
	const uint32_t bpp = data_format == GL_RGBA ? 4 : 3;
	assert(size == width * height * bpp);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, data_format, GL_UNSIGNED_BYTE, data);
}

Texture2D::~Texture2D()
{
	glDeleteTextures(1, &texture);
}

void Texture2D::bind(RenderState& gfx)
{
	assert(slot >= 0 && slot < 32);
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, texture);
}
void Texture2D::unbind(RenderState& gfx)
{
	assert(slot >= 0 && slot < 32);
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, 0);
}
