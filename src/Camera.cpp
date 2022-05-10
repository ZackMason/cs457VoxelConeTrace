#include "Camera.hpp"

#include "glm/gtc/matrix_transform.hpp"

Camera::Camera(f32 f, f32 w, f32 h, f32 n, f32 fa)
:
fov(f), width(w), height(h), near(n), far(fa)
{
	projection = glm::perspectiveFov(fov, width, height, near, far);
}

void Camera::LookAt(const v3f& target, const v3f& up)
{
	//view.LookAt(pos, up);
	view = glm::lookAt(position, target, up);
}

void Camera::MakeProjection()
{
	projection = glm::perspectiveFov(fov, width, height, near, far);
}
