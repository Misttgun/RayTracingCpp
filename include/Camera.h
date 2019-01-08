#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include <utility>
#include "Entity.h"
#include "Ray.h"

class Camera : public Entity
{
public:
	Camera() = default;
	Camera(const float x, const float y, const float z);
	Camera(const float x, const float y, const float z, const float v_focal);
	Camera(const Camera& c);
	~Camera() = default;

	Camera& operator= (const Camera& rhs);

	Ray get_ray(const float x, const float y) const;

	friend void swap(Camera& first, Camera& second) noexcept
	{
		using std::swap;
		swap(static_cast<Entity&>(first), static_cast<Entity&>(second));
		swap(first.focal, second.focal);
	}
	
	float focal;
};
#endif