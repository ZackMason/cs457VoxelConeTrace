#pragma once

#include "Core.hpp"
#include "AABB.hpp"

struct Transform
{
	Transform() = default;
	Transform(const m44& mat) : basis(mat), origin(mat[3]) {};
	Transform(const v3f& position, const v3f& scale, const v3f& rotation)
	{
		set_position(position);
		set_scale(scale);
		set_rotation(position);
	}
    
	m44 to_matrix() const;
    
	operator m44() const {
		return to_matrix();
	}
    
	void translate(const v3f& delta);
	void scale(const v3f& delta);
	void rotate(const v3f& delta);
	void rotate(const glm::quat& delta);
    
	v3f get_origin() const;
    
	Transform inverse() const;
    
	void set_fov(f32 fov, f32 width, f32 height, f32 near, f32 far);
	void look_at(const v3f& target, const v3f& up = { 0,1,0 });
    
	void set_position(const v3f& position);
	void set_scale(const v3f& scale);
	void set_rotation(const v3f& rotation);
	void set_rotation(const glm::quat& quat);
    
	v3f xform(const v3f& vector)const
	{
		return v3f(basis * vector) + origin;
	}
    
	AABB xform(const AABB& box)const
	{
		AABB t_box;
		t_box.min = t_box.max = origin;
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
        {
            auto a = basis[j][i] * box.min[j];
            auto b = basis[j][i] * box.max[j];
            t_box.min[i] += a < b ? a : b;
            t_box.max[i] += a < b ? b : a;
        }
        
		//t_box.min = xform(box.min);
		//t_box.max = xform(box.max);
		return t_box;
	}
    
	m33 basis = m33(1.0f);
	v3f origin = v3f(0, 0, 0);
};
