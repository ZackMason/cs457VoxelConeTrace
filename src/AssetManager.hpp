#pragma once

#include "Mesh.hpp"
#include "Shader.hpp"
#include "Texture2D.hpp"

struct AssetManager
{
    using MultiMeshCache = std::unordered_map<std::string, MultiMesh*>;
	using Texture2DCache = std::unordered_map<std::string, Texture2D*>;
	using ShaderCache = std::unordered_map<std::string, Shader*>;
    
	static inline MultiMeshCache model_cache;
	static inline Texture2DCache texture2d_cache;
	static inline ShaderCache shader_cache;
    
	static MultiMesh* get_model(const std::string& filename)
	{
		if (model_cache.count(filename))
			return model_cache[filename];
        
		std::stringstream fs; fs << ASSETS_PATH << filename;
        
		model_cache[filename] = new MultiMesh(fs.str().c_str());
		return model_cache[filename];
	}
	
	static Texture2D* get_texture2d(const std::string& filename)
	{
		if (texture2d_cache.count(filename))
			return texture2d_cache[filename];
		std::stringstream fs; fs << ASSETS_PATH << filename;
        
		return texture2d_cache[filename] = new Texture2D(fs.str().c_str());
	}
	
	static Shader* get_shader(const std::string& filename)
	{
		if (shader_cache.count(filename))
			return shader_cache[filename];
        
		std::stringstream fs; fs << ASSETS_PATH << filename << ".fs";
		std::stringstream vs; vs << ASSETS_PATH << filename << ".vs";
        
		return shader_cache[filename] = new Shader(filename, {vs.str().c_str(), fs.str().c_str()});
	}
};