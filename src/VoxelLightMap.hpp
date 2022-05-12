#pragma once

#include "Texture3D.hpp"
#include "Shader.hpp"
#include "Camera.hpp"

struct VoxelLightMap
{
    int size = 512/2;
    Texture3D* voxel_map = nullptr;
    Shader* forward_pass_shader = new Shader("voxel_forward", {"voxel_light.vs", "voxel_light.fs"});
    Shader voxel_pass_shader("voxel_pass", {"voxel.vs", "voxel.gs", "voxel.fs"});
    Camera voxel_camera;

    constexpr auto voxel_pass()
    {

    }

    VoxelLightMap(int p_size = 512/2)
        : size(p_size)
    {
        voxel_map = new Texture3D(size, size, size);
    }

    virtual ~VoxelLightMap()
    {
        delete forward_pass_shader;
        delete voxel_map;
    }
};