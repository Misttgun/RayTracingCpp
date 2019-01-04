#include "Ray.h"

#include <utility>

Ray::Ray(const Point& p, const Vector& d)
	:origin(p), direction(d)
{}

Ray::Ray(float ox, float oy, float oz, float dx, float dy, float dz)
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

void Ray::swap(Ray& first, Ray& second)
{
	std::swap(first.origin, second.origin);
	std::swap(first.origin, second.origin);
}
