#pragma once
#ifndef LIGHT_H
#define LIGHT_H

#include <utility>
#include "Color.h"
#include "Entity.h"
#include "Ray.h"

class Light : public Entity
{
public:
	Light() = default;
	Light(const float x, const float y, const float z);
	Light(const float x, const float y, const float z, const Color& v_color);
	Light(const Light& c);
	~Light() = default;

	Light& operator= (const Light& rhs);

	Ray get_ray_to_light(const Vector& p) const;
	Ray get_ray_from_light(const Vector& p) const;
	Vector get_vector_to_light(const Vector& p) const;
	Vector get_vector_from_light(const Vector& p) const;

	friend void swap(Light& first, Light& second) noexcept
	{
		using std::swap;
		swap(static_cast<Entity&>(first), static_cast<Entity&>(second));
		swap(first.color, second.color);
	}

	Color color;
};
#endif