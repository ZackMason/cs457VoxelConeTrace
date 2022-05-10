#pragma once


#include "Core.hpp"
#include "RenderState.hpp"
#include "AABB.hpp"


struct Vertex {
    v3f Position;
    v3f Normal;
    v2f TexCoords;
	//glm::vec3 Tangent;
	//glm::vec3 Bitangent;
};

struct Texture2D;

struct StaticMesh 
{
	StaticMesh(const std::string& file_name);
	StaticMesh(const std::vector<Vertex>& verts) : vertices(verts) {create();};
    
	void load_vbo();
	void create()
    {
        glGenVertexArrays(1, &vao);
        
        glBindVertexArray(vao);
        glGenBuffers(1, &vbo);
        
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
        
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
        
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, Normal)));
        
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, TexCoords)));
        
        //glEnableVertexAttribArray(3);
        //glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, Tangent)));
        
        //glEnableVertexAttribArray(4);
        //glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, Bitangent)));
        
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        
    }
	void draw(RenderState& gfx)
    {
        bind(gfx);
        glDrawArrays(GL_TRIANGLES, 0, size);
        unbind(gfx);
    }
	void bind(RenderState& gfx)
    {
        glBindVertexArray(vao);
    }
	void unbind(RenderState& gfx)
    {
        glBindVertexArray(0);
    }
    
    void update_aabb()
    {
        bounding_box.min = v3f(std::numeric_limits<float>::max());
        bounding_box.max = v3f(std::numeric_limits<float>::min());
        
        for (const auto& vert : vertices)
        {
            bounding_box.min = glm::min(bounding_box.min, vert.Position);
            bounding_box.max = glm::max(bounding_box.max, vert.Position);
        }
    }
	
    GLuint vbo;
	GLuint vao;
    GLsizei size;
	std::string path;
    
    std::vector<Texture2D*> textures;
	std::vector<Vertex> vertices;
    
    AABB bounding_box;
};

struct Material
{
    Texture2D* diffuse = nullptr;
    Texture2D* normal = nullptr;
};

struct SubMesh
{
    SubMesh(const std::vector<Vertex>& verts) : vertices(verts) { create(); };
    
    void load_vbo();
    void create()
    {
        glGenVertexArrays(1, &vao);
        
        glBindVertexArray(vao);
        glGenBuffers(1, &vbo);
        
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
        
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
        
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, Normal)));
        
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, TexCoords)));
        
        //glEnableVertexAttribArray(3);
        //glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, Tangent)));
        
        //glEnableVertexAttribArray(4);
        //glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, Bitangent)));
        
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        
        size = (GLsizei)vertices.size();
    }
    void draw(RenderState& gfx)
    {
        bind(gfx);
        glDrawArrays(GL_TRIANGLES, 0, size);
        unbind(gfx);
    }
    void bind(RenderState& gfx)
    {
        glBindVertexArray(vao);
    }
    void unbind(RenderState& gfx)
    {
        glBindVertexArray(0);
    }
    
    void update_aabb()
    {
        bounding_box.min = v3f(std::numeric_limits<float>::max());
        bounding_box.max = v3f(std::numeric_limits<float>::min());
        
        for (const auto& vert : vertices)
        {
            bounding_box.min = glm::min(bounding_box.min, vert.Position);
            bounding_box.max = glm::max(bounding_box.max, vert.Position);
        }
    }
    
    GLuint vbo;
    GLuint vao;
    GLsizei size;
    
    Material textures;
    std::vector<Vertex> vertices;
    
    AABB bounding_box;
};

struct MultiMesh
{
    MultiMesh(const std::string& filename);
    ~MultiMesh() {
        for (auto submesh : meshes)
        {
            delete submesh;
        }
    }
    
    void update_aabb()
    {
        bounding_box.min = v3f(std::numeric_limits<float>::max());
        bounding_box.max = v3f(std::numeric_limits<float>::min());
        
        for (auto submesh : meshes)
        {
            bounding_box.min = glm::min(bounding_box.min, submesh->bounding_box.min);
            bounding_box.max = glm::max(bounding_box.max, submesh->bounding_box.max);
        }
    }
    
    std::string path;
    
    std::vector<SubMesh*> meshes;
    
    AABB bounding_box;
    
    
    
};