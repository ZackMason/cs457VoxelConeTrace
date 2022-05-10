#pragma once

#include <glad/glad.h> 
#include "Core.hpp"
#include <cassert>
#include <regex>
#include "imgui.h"

enum class eUniformType {
    BOOL, INT, FLOAT, VEC2, VEC3, VEC4, MAT2, MAT3, MAT4, SAMPLER2D, INVALID
};

struct UniformVariable
{
    std::shared_ptr<void> data;
    eUniformType type;
    std::string name;
    float min = 0.1f;
    float max = 1.0f;
    float step = 0.1f;
};

struct ShaderStage{
    ShaderStage(const std::string& file_name)
    {
        if (file_name.substr(file_name.find_last_of(".") + 1) == "vs")
        {
            type = GL_VERTEX_SHADER;
        }
        else if (file_name.substr(file_name.find_last_of(".") + 1) == "fs")
        {
            type = GL_FRAGMENT_SHADER;
        }
        else if (file_name.substr(file_name.find_last_of(".") + 1) == "gs")
        {
            type = GL_GEOMETRY_SHADER;
        }
        else
        {
            assert(0); // shader type not supported
        }
        
        // 1. retrieve the vertex/fragment source code from filePath
        std::string shader_code;
        std::ifstream shader_file;
        // ensure ifstream objects can throw exceptions:
        shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        
        std::string shader_path = ASSETS_PATH + file_name;
        try
        {
            // open 
            shader_file.open(shader_path);
            std::stringstream shader_stream;
            // read file's buffer contents into streams
            shader_stream << shader_file.rdbuf();
            // close file handlers
            shader_file.close();
            // convert stream into string
            shader_code = shader_stream.str();
            
            //IncludePreprocess(shader_code);
        }
        catch (std::ifstream::failure e)
        {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ: " << file_name << " : " << e.what() << "\n";
        }
        
        const char* vShaderCode = shader_code.c_str();
        code = shader_code;
        
        int shader_slot = glCreateShader(type);
        glShaderSource(shader_slot, 1, &vShaderCode, NULL);
        glCompileShader(shader_slot);
        // check for shader compile errors
        int success;
        char infoLog[512];
        glGetShaderiv(shader_slot, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader_slot, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::" << file_name << "::" << type << "::COMPILATION_FAILED: " << infoLog << "\n";
        }
        else
        {
            stage = shader_slot;
        }
    }
    
    std::vector<std::array<std::string, 7>> get_uniforms()
    {
        std::smatch m;
        //std::regex e(R"rgx(^uniform (\w+) (\w+);\s*\/\/\s*auto)rgx");
        std::regex e(R"rgx(^uniform (\w+) (\w+);\s*\/\/\s*(?:auto|export)\s*(\[\s*([-+]?[0-9]*\.?[0-9]+)\s*,\s*([-+]?[0-9]*\.?[0-9]+)\s*,\s*([-+]?[0-9]*\.?[0-9]+)\s*\])?\s*([-+]?[0-9]*\.?[0-9]+)?)rgx");
        std::vector<std::array<std::string, 7>> res;
        
        auto code_copy = code;
        
        while (std::regex_search(code_copy, m, e))
        {
            res.push_back({ m[0], m[1], m[2], m[4], m[5], m[6], m[7] });
            
            code_copy = m.suffix().str();
        }
        return res;
    }
    
    operator int() { return type; }
    
    std::string code;
    
    int type;
    int stage;
};


struct Shader
{
    std::vector<ShaderStage*> stages;
	Shader() = delete;
	~Shader(){
        for (auto& stage : stages) delete stage;
        glDeleteProgram(program);
    }
	Shader(const std::string& name, const std::vector<std::string>& files)
    {
        program = glCreateProgram();
        for (const auto& file : files)
        {
            auto stage = new ShaderStage(file);
            glAttachShader(program, stage->stage);
            stages.push_back(stage);
        }
        int success;
        char errorLog[512];
        
        glLinkProgram(program);
        
        glGetProgramiv(program, GL_LINK_STATUS, &success);
        if(!success)
        {
            glGetProgramInfoLog(program, 512, nullptr, errorLog);
            std::cerr << "ERROR::SHADER::" << name << "::PROGRAM::LINKING_FAILED: " << errorLog << std::endl;
        }
        else
        {
            std::cout << "LOADED SHADER: " << name << std::endl;
        }
    }
    
	operator int() { return program; }
    
    std::vector<std::array<std::string, 7>> get_uniforms();
    std::vector<UniformVariable> get_uniform_variables();
    void uniform_edit(std::vector<UniformVariable>& uniforms);
    void set_uniforms(std::vector<UniformVariable>& uniforms);
    
	void bind()
    {
        glUseProgram(program);
    }
    
	GLint get_uniform_location(const std::string& name) const
    {
        if (uniform_location_cache.count(name))
            return uniform_location_cache[name];
        GLint location = glGetUniformLocation(program, name.c_str());
        uniform_location_cache[name] = location;
        return location;
    }
    
	mutable std::unordered_map<std::string, GLint> uniform_location_cache;
    
	std::string name;
	int program;
    
	// utility uniform functions
    // --------------------------------------------------------------------------
    void set_uniform_block(const std::string &name, int loc) const;
	void set_bool  (const std::string &name, bool  value) const;
	void set_int   (const std::string& name, int   value) const;
	void set_uint  (const std::string &name, unsigned int   value) const;
	void set_float (const std::string &name, float value) const;
	void set_vec2  (const std::string &name, float x, float y) const;
	void set_vec3  (const std::string &name, float x, float y, float z) const;
	void set_vec4  (const std::string &name, float x, float y, float z, float w);
	void set_vec2  (const std::string &name, const v2f &value) const;
	void set_vec3  (const std::string &name, const v3f &value) const;
	void set_vec4  (const std::string &name, const v4f &value) const;
	void set_mat2  (const std::string &name, const m22 &mat) const;
	void set_mat3  (const std::string &name, const m33 &mat) const;
	void set_mat4  (const std::string &name, const m44 &mat) const;
	
    // --------------------------------------------------------------------------
};

