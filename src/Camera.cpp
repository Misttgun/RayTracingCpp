#include "Camera.h"

Camera::Camera(const float x, const float y, const float z)
    : Entity(x, y, z), focal(0)
{
}

Camera::Camera(const float x, const float y, const float z, const float v_focal)
	: Entity(x, y, z), focal(v_focal)
{}

Camera::Camera(const Camera & c)
	: Entity(c)
{
	focal = c.focal;
}

Camera & Camera::operator=(const Camera & rhs)
{
	Camera temp(rhs);
	swap(temp, *this);
	return *this;
}

Ray Camera::get_ray(const float x, const float y) const
{
    const float screen_x = (2 * x) - 1;
    const float screen_y = (2 * y) - 1;

    const Vector ori(screen_x, screen_y, 0);
    const Vector cam_pos(0, 0, focal);

    const Vector direction = local_to_global_vector(ori - cam_pos).normalized();

	return Ray(local_to_global_point(ori), direction);
}
