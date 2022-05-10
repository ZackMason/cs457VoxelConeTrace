#include "Shader.hpp"


std::vector<std::array<std::string, 7>> Shader::get_uniforms()
{
    std::vector<std::array<std::string, 7>> res;
    for (auto stage : stages)
    {
        auto r = stage->get_uniforms();
        res.insert(res.end(), r.begin(), r.end());
    }
    return res;
}



std::vector<UniformVariable> Shader::get_uniform_variables()
{
    std::vector<UniformVariable> result;
    
    for(const auto& uniform_array : get_uniforms())
    {
        const auto& [full, type, name, min, step, max, start] = uniform_array;
        UniformVariable var;
        var.name = name;
        
        if (min != "")
            var.min = std::atof(min.c_str());
        if (max != "")
            var.max = std::atof(max.c_str());
        if (step != "")
            var.step = std::atof(step.c_str());
        
        if (type == "bool") {
            var.data = std::make_shared<bool>(start != "" ? std::atoi(start.c_str()) : false);
            var.type = eUniformType::BOOL;
            
        }
        else if (type == "int") {
            var.data = std::make_shared<int>(start != "" ? std::atoi(start.c_str()) : 1);
            var.type = eUniformType::INT;
        }
        else if (type == "float") {
            var.data = std::make_shared<float>(start != "" ? std::atof(start.c_str()) : 1.0f);
            var.type = eUniformType::FLOAT;
        }
        else if (type == "vec3") {
            var.data = std::make_shared <v3f>(start != "" ? std::atof(start.c_str()) : 0.0f);
            var.type = eUniformType::VEC3;
        }
        else if (type == "vec4") {
            var.data = std::make_shared<v4f>(start != "" ? std::atof(start.c_str()) : 0.0f);
            var.type = eUniformType::VEC4;
        }
        else if (type == "vec2") {
            var.data = std::make_shared<v2f>(start != "" ? std::atof(start.c_str()) : 0.0f);
            var.type = eUniformType::VEC2;
        }
        else {
            var.type = eUniformType::INVALID;
            
        }
        
        result.push_back(var);
    }
    return result;
}

void Shader::uniform_edit(std::vector<UniformVariable>& uniforms)
{
    for (auto& uniform : uniforms)
    {
        switch (uniform.type)
        {
            case eUniformType::BOOL:
            ImGui::Checkbox(uniform.name.c_str(), (bool*)uniform.data.get());
            break;
            case eUniformType::INT:
            ImGui::DragInt(uniform.name.c_str(), (int*)uniform.data.get(), uniform.step, uniform.min, uniform.max );
            break;
            case eUniformType::FLOAT:
            ImGui::DragFloat(uniform.name.c_str(), (f32*)uniform.data.get(), uniform.step, uniform.min, uniform.max);
            break;
            case eUniformType::VEC3:
            ImGui::DragFloat3(uniform.name.c_str(), glm::value_ptr(*(glm::vec3*)uniform.data.get()));
            break;
            case eUniformType::VEC4:
            ImGui::ColorEdit4(uniform.name.c_str(), glm::value_ptr(*(glm::vec4*)uniform.data.get()));
            break;
            default:
            ImGui::Text("Unimplemented uniform: %s", uniform.name.c_str());
            break;
        }
    }
}

void Shader::set_uniforms(std::vector<UniformVariable>& uniforms)
{
    for (auto& uniform : uniforms)
    {
        switch (uniform.type)
        {
            case eUniformType::BOOL:
            set_bool(uniform.name, *(bool*)uniform.data.get());
            break;
            case eUniformType::INT:
            set_int(uniform.name, *(int*)uniform.data.get());
            break;
            case eUniformType::FLOAT:
            set_float(uniform.name, *(f32*)uniform.data.get());
            break;
            case eUniformType::VEC3:
            set_vec3(uniform.name, *(glm::vec3*)uniform.data.get());
            break;
            case eUniformType::VEC4:
            set_vec4(uniform.name, *(glm::vec4*)uniform.data.get());
            break;
            default:
            break;
        }
    }
}

void Shader::set_bool(const std::string &name, bool value) const
{
	glUniform1i(get_uniform_location(name), (int)value);
}
// ------------------------------------------------------------------------
void Shader::set_int(const std::string &name, int value) const
{
	glUniform1i(get_uniform_location(name), value);
}
// ------------------------------------------------------------------------
void Shader::set_uint(const std::string& name, unsigned int value) const
{
	glUniform1ui(get_uniform_location(name), value);
}
// ------------------------------------------------------------------------
void Shader::set_float(const std::string &name, float value) const
{
	glUniform1f(get_uniform_location(name), value);
}
// ------------------------------------------------------------------------
void Shader::set_vec2(const std::string &name, const v2f &value) const
{
	glUniform2fv(get_uniform_location(name), 1, &value[0]);
}
void Shader::set_vec2(const std::string &name, float x, float y) const
{
	glUniform2f(get_uniform_location(name), x, y);
}
// ------------------------------------------------------------------------
void Shader::set_vec3(const std::string &name, const v3f &value) const
{
	glUniform3fv(get_uniform_location(name), 1, &value[0]);
}
void Shader::set_vec3(const std::string &name, float x, float y, float z) const
{
	glUniform3f(get_uniform_location(name), x, y, z);
}
// ------------------------------------------------------------------------
void Shader::set_vec4(const std::string &name, const v4f &value) const
{
	glUniform4fv(get_uniform_location(name), 1, &value[0]);
}
void Shader::set_vec4(const std::string &name, float x, float y, float z, float w)
{
	glUniform4f(get_uniform_location(name), x, y, z, w);
}
// ------------------------------------------------------------------------
void Shader::set_mat2(const std::string &name, const m22 &mat) const
{
	glUniformMatrix2fv(get_uniform_location(name), 1, GL_FALSE, &mat[0][0]);
}
// ------------------------------------------------------------------------
void Shader::set_mat3(const std::string &name, const m33 &mat) const
{
	glUniformMatrix3fv(get_uniform_location(name), 1, GL_FALSE, &mat[0][0]);
}
// ------------------------------------------------------------------------
void Shader::set_mat4(const std::string &name, const m44 &mat) const
{
	glUniformMatrix4fv(get_uniform_location(name), 1, GL_FALSE, &mat[0][0]);
}
