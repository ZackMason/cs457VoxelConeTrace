#pragma once

#include "Ray.hpp"

struct uCamera
{
	glm::mat4 v;
	glm::mat4 p;
	glm::vec4 camPos;
};

struct Camera
{
	Camera(f32 f, f32 w, f32 h, f32 n, f32 fa);
    
	void LookAt(const v3f& pos, const v3f& up = { 0,1,0 });
    
	m44 ViewProjection() const
	{
		return projection * view;
	}
    
	void MakeProjection();
    
	auto GetForward() const {
		return -glm::normalize(v3f(glm::transpose(view)[2]));  //.basis[2]));
	}
    
	auto GetRight() const {
		return -glm::normalize(v3f(glm::transpose(view)[0]));  //.basis[2]));
	}
    auto GetUp() const {
		return -glm::normalize(v3f(glm::transpose(view)[1]));  //.basis[2]));
	}
    
	auto GetPosition()const {
		return position;// view.origin;
	}
    
	Ray GetScreenRay(u32 x, u32 y)
	{
		v4f ndc = {
			(2.0f * x) / width - 1.0f,
			1.0f - (2.0f * y) / height,
			-1.0f, 1.0f,
		};
        
		v4f ray_eye = glm::inverse(projection) * ndc;
		ray_eye.z = -1.0f;
		ray_eye.w = 0.0f;
		return { position, glm::normalize(v3f(glm::inverse(view) * ray_eye)) };
	}
    
	f32 fov;
	f32 width;
	f32 height; 
	f32 near = 0.01f;
	f32 far = 1000.f;
    
	v3f position = { 0,0,0 };
	m44 view;
	//Transform view;
	m44 projection;
};