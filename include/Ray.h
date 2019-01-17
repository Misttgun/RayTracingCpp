#ifndef RAY_H
#define RAY_H

#include "Vector.h"

class Ray
{
public:
	Ray(const Vector& p, const Vector& d);
	Ray(float ox, float oy, float oz, float dx, float dy, float dz);
	Ray(const Ray &ray);
	Ray& operator=(const Ray &ray);
	~Ray() = default;

	friend void swap(Ray& first, Ray& second) noexcept
	{
		using std::swap;
		swap(first.origin, second.origin);
		swap(first.origin, second.origin);
	}

	Vector origin;
	Vector direction;
};

#endif 