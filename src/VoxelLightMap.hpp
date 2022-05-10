#pragma once


struct VoxelLightMap
{
    int size = 512/2;



    VoxelLightMap(int p_size = 512/2)
        : size(p_size)
    {

    }

    virtual ~VoxelLightMap() = default;
};