#include "Mesh.hpp"
#include "AssetManager.hpp"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

StaticMesh::StaticMesh(const std::string& file_path) : path(file_path)
{
    std::vector<unsigned int> indices;
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    
    std::string warn;
    std::string err;
    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, path.c_str(), ASSETS_PATH);
    
    if (!warn.empty())
        std::cout << "MESH WARNING: " << path << " : " << warn << std::endl;
    if (!err.empty())
        std::cout << "MESH ERROR: " << path << " : " << err << std::endl;
    if (!ret)
    {
        std::cout << "MESH ERROR: FAILED TO LOAD MESH " << path << ", CHECK RESOURCE FOLDER" << std::endl;
    }
    size = 0;
    
    
    for (size_t s = 0; s < shapes.size(); s++)
    {
        // Loop over faces(polygon)
        size_t index_offset = 0;
        for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++)
        {
            int fv = shapes[s].mesh.num_face_vertices[f];
            
            // Loop over vertices in the face.
            for (int v = 0; v < fv; v++) {
                // access to vertex
                tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
                tinyobj::real_t vx = attrib.vertices[3 * idx.vertex_index + 0];
                tinyobj::real_t vy = attrib.vertices[3 * idx.vertex_index + 1];
                tinyobj::real_t vz = attrib.vertices[3 * idx.vertex_index + 2];
                tinyobj::real_t nx = attrib.normals[3 * idx.normal_index + 0];
                tinyobj::real_t ny = attrib.normals[3 * idx.normal_index + 1];
                tinyobj::real_t nz = attrib.normals[3 * idx.normal_index + 2];
                tinyobj::real_t tx = attrib.texcoords[2 * idx.texcoord_index + 0];
                tinyobj::real_t ty = attrib.texcoords[2 * idx.texcoord_index + 1];
                
                Vertex vert;
                vert.Position = { vx,vy,vz };
                vert.Normal = { nx,ny,nz };
                vert.TexCoords = { tx,ty };
                
                vertices.push_back(vert);
                size++;
                //indices.push_back(idx.vertex_index);
            }
            
#if 0                
            auto iv1 = vertices.size() - 3;
            v3f v1 = vertices[iv1 + 0].Position;
            v3f v2 = vertices[iv1 + 1].Position;
            v3f v3 = vertices[iv1 + 2].Position;
            v2f uv1 = vertices[iv1 + 0].TexCoords;
            v2f uv2 = vertices[iv1 + 1].TexCoords;
            v2f uv3 = vertices[iv1 + 2].TexCoords;
            
            auto e1 = v2 - v1;
            auto e2 = v3 - v1;
            
            auto duv1 = uv2 - uv1;
            auto duv2 = uv3 - uv1;
            
            float det = 1.0f / (duv1.x * duv2.y - duv2.x * duv1.y);
            
            v3f t, b;
            
            t.x = det * (duv2.y * e1.x - duv1.y * e2.x);
            t.y = det * (duv2.y * e1.y - duv1.y * e2.y);
            t.z = det * (duv2.y * e1.z - duv1.y * e2.z);
            t = normalize(t);
            
            b.x = det * (-duv2.x * e1.x + duv1.x * e2.x);
            b.y = det * (-duv2.x * e1.y + duv1.x * e2.y);
            b.z = det * (-duv2.x * e1.z + duv1.x * e2.z);
            b = normalize(b);
            
            vertices[iv1 + 0].Tangent = t;
            vertices[iv1 + 1].Tangent = t;
            vertices[iv1 + 2].Tangent = t;
            
            vertices[iv1 + 0].Bitangent = b;
            vertices[iv1 + 1].Bitangent = b;
            vertices[iv1 + 2].Bitangent = b;
#endif
            
            index_offset += fv;
        }
    }
    std::cout << "LOADED STATICMESH: " << path << "\n";
    create();
}

MultiMesh::MultiMesh(const std::string& filename)
: path(filename)
{
    
    std::vector<unsigned int> indices;
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    
    std::string warn;
    std::string err;
    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, path.c_str(), ASSETS_PATH);
    
    if (!warn.empty())
        std::cout << "MESH WARNING: " << path << " : " << warn << std::endl;
    if (!err.empty())
        std::cout << "MESH ERROR: " << path << " : " << err << std::endl;
    if (!ret)
    {
        std::cout << "MESH ERROR: FAILED TO LOAD MESH " << path << ", CHECK RESOURCE FOLDER\n";
    }
    
    
    for (size_t s = 0; s < shapes.size(); s++)
    {
        std::vector<Vertex> submesh_verts;
        int size = 0;
        // Loop over faces(polygon)
        size_t index_offset = 0;
        for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++)
        {
            int fv = shapes[s].mesh.num_face_vertices[f];
            
            // Loop over vertices in the face.
            for (int v = 0; v < fv; v++) {
                // access to vertex
                tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
                tinyobj::real_t vx = attrib.vertices[3 * idx.vertex_index + 0];
                tinyobj::real_t vy = attrib.vertices[3 * idx.vertex_index + 1];
                tinyobj::real_t vz = attrib.vertices[3 * idx.vertex_index + 2];
                tinyobj::real_t nx = attrib.normals[3 * idx.normal_index + 0];
                tinyobj::real_t ny = attrib.normals[3 * idx.normal_index + 1];
                tinyobj::real_t nz = attrib.normals[3 * idx.normal_index + 2];
                tinyobj::real_t tx = attrib.texcoords[2 * idx.texcoord_index + 0];
                tinyobj::real_t ty = attrib.texcoords[2 * idx.texcoord_index + 1];
                
                Vertex vert;
                vert.Position = { vx,vy,vz };
                vert.Normal = { nx,ny,nz };
                vert.TexCoords = { tx,ty };
                
                submesh_verts.push_back(vert);
                size++;
                //indices.push_back(idx.vertex_index);
            }
            
#if 0                
            auto iv1 = vertices.size() - 3;
            v3f v1 = vertices[iv1 + 0].Position;
            v3f v2 = vertices[iv1 + 1].Position;
            v3f v3 = vertices[iv1 + 2].Position;
            v2f uv1 = vertices[iv1 + 0].TexCoords;
            v2f uv2 = vertices[iv1 + 1].TexCoords;
            v2f uv3 = vertices[iv1 + 2].TexCoords;
            
            auto e1 = v2 - v1;
            auto e2 = v3 - v1;
            
            auto duv1 = uv2 - uv1;
            auto duv2 = uv3 - uv1;
            
            float det = 1.0f / (duv1.x * duv2.y - duv2.x * duv1.y);
            
            v3f t, b;
            
            t.x = det * (duv2.y * e1.x - duv1.y * e2.x);
            t.y = det * (duv2.y * e1.y - duv1.y * e2.y);
            t.z = det * (duv2.y * e1.z - duv1.y * e2.z);
            t = normalize(t);
            
            b.x = det * (-duv2.x * e1.x + duv1.x * e2.x);
            b.y = det * (-duv2.x * e1.y + duv1.x * e2.y);
            b.z = det * (-duv2.x * e1.z + duv1.x * e2.z);
            b = normalize(b);
            
            vertices[iv1 + 0].Tangent = t;
            vertices[iv1 + 1].Tangent = t;
            vertices[iv1 + 2].Tangent = t;
            
            vertices[iv1 + 0].Bitangent = b;
            vertices[iv1 + 1].Bitangent = b;
            vertices[iv1 + 2].Bitangent = b;
#endif
            
            index_offset += fv;
        }
        meshes.push_back(new SubMesh(submesh_verts));
        
        auto& mesh = meshes.back();
        mesh->update_aabb();
        
        for (auto mat_id : shapes[s].mesh.material_ids)
        {
            
            auto material = materials[mat_id];
            if (material.diffuse_texname != "")
                mesh->textures.diffuse = (AssetManager::get_texture2d(material.diffuse_texname));
            if (material.displacement_texname != "")
                mesh->textures.normal = AssetManager::get_texture2d(material.displacement_texname);
            else if (material.normal_texname != "")
                mesh->textures.normal = AssetManager::get_texture2d(material.normal_texname);
        }
    }
    update_aabb();
    std::cout << "LOADED MULTIMESH: " << path << std::endl;
}