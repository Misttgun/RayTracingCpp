#include "Ray.h"

#include <utility>

Ray::Ray(const Vector& p, const Vector& d)
	:origin(p), direction(d)
{}

Ray::Ray(const float ox, const float oy, const float oz, const float dx, const float dy, const float dz)
	: origin(ox, oy, oz), direction(dx, dy, dz)
{}

Ray::Ray(const Ray& ray)
{
	origin = ray.origin;
	direction = ray.direction;
}

Ray& Ray::operator=(const Ray& ray)
{
	Ray temp(ray);
	swap(*this, temp);
	return *this;
}