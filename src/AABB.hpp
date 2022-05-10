#pragma once
#include "Core.hpp"
#include "IntersectData.hpp"
#include "Ray.hpp"

struct AABB
{
	v3f min;
	v3f max;
    
	bool contains(const v3f& p)
	{
		return (p.x < max.x && p.x > min.x) && 
        (p.y < max.y && p.y > min.y) &&
        (p.z < max.z && p.z > min.z);
	}
    
	IntersectData intersect_ray(const Ray& ray, Node* node = nullptr)
	{
		if (contains(ray.origin)) return { true , 0.0, node, 0.0};
        
		v3f inv_dir = 1.0f / ray.direction;
        
		v3f min_t = (min - ray.origin) * inv_dir;
		v3f max_t = (max - ray.origin) * inv_dir;
        
		f32 tmin = std::max(std::max(std::min(min_t.x, max_t.x), std::min(min_t.y, max_t.y)), std::min(min_t.z, max_t.z));
		f32 tmax = std::min(std::min(std::max(min_t.x, max_t.x), std::max(min_t.y, max_t.y)), std::max(min_t.z, max_t.z));
        
		if (tmax < 0.0f)
		{
			return { false, 0, nullptr, 0.0 };
		}
		if (tmin > tmax)
		{
			return { false, 0, nullptr, 0.0 };
		}
		return { true, tmin, node, 0.0, ray.origin + ray.direction*tmin };
	}
};