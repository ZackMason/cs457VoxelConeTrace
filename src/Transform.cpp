#include "Transform.hpp"

#include "glm/gtc/matrix_transform.hpp"

m44 Transform::to_matrix() const
{
	m44 res = basis;
	res[3] = v4f(origin,1.0);
	return res;
}

void Transform::translate(const v3f& delta)
{
	origin += delta;
}

void Transform::scale(const v3f& delta)
{
	basis = glm::scale(m44(basis), delta);
}

void Transform::rotate(const v3f& delta)
{
	m44 rot = m44(1.0f);
	rot = glm::rotate(rot, delta.z, { 0,0,1 });
	rot = glm::rotate(rot, delta.y, { 0,1,0 });
	rot = glm::rotate(rot, delta.x, { 1,0,0 });
	basis = m44(basis) * rot;
}


void Transform::rotate(const glm::quat& delta)
{
    
}

v3f Transform::get_origin() const
{
	return origin;
}

Transform Transform::inverse() const
{
	Transform transform;
	transform.basis = glm::transpose(basis);
	transform.origin = basis * -origin;
	return transform;
}

void Transform::set_fov(f32 fov, f32 width, f32 height, f32 near, f32 far)
{
	auto mat = glm::perspectiveFov(fov, width, height, near, far);
	basis = mat;
	origin = mat[3];
}

void Transform::look_at(const v3f& target, const v3f& up)
{
	auto mat = glm::lookAt(origin, target, up);
	basis = mat;
	origin = mat[3];
}

void Transform::set_position(const v3f& position)
{
	origin = position;
}

void Transform::set_scale(const v3f& scale)
{
	for (int i = 0; i < 3; i++)
		basis[i] = glm::normalize(basis[i]) * scale[i];
}

void Transform::set_rotation(const v3f& rotation)
{
}

void Transform::set_rotation(const glm::quat& quat)
{
}
