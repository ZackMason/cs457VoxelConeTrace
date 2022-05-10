#pragma once

#include "AssetManager.hpp"
#include "Texture3D.hpp"
#include "Screen.hpp"
#include "RenderState.hpp"

struct Scene 
{
    
    static Scene* voxel_cone_trace();
    static Scene* image_manipulation();
    static Scene* tiger();
    static Scene* quantize();
    
    virtual void run(RenderState&) = 0;
    
};

struct VoxelScene : Scene
{
    MultiMesh sponza = MultiMesh("./assets/sponza.obj");
    Shader base = Shader("base", {"base.vs", "base.fs"});
    Shader voxel_pass = Shader("voxel", {"voxel.vs", "voxel.fs", "voxel.gs"});
    
    int voxel_size = 256;
    Texture3D voxel_map = Texture3D(voxel_size, voxel_size, voxel_size);
    
    virtual void run(RenderState&);
};

