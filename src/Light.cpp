#include "Light.h"

Light::Light(const float x, const float y, const float z)
	: Entity(x, y, z)
{}

Light::Light(const float x, const float y, const float z, const Color & v_id, const Color & v_is)
	: Entity(x, y, z), id(v_id), is(v_is)
{}

Light::Light(const Light & c)
	: Entity(c)
{
	id = c.id;
	is = c.is;
}

Light& Light::operator=(const Light& rhs)
{
	Light temp(rhs);
	swap(temp, *this);
	return *this;
}

Ray Light::get_ray_to_light(const Point& p) const
{
	Vector dir = position - p;
	return Ray(p, dir);
}

Ray Light::get_ray_from_light(const Point& p) const
{
	Vector dir = p - position;
	return Ray(position, dir);
}

Vector Light::get_vector_to_light(const Point& p) const
{
	return position - p;
}

Vector Light::get_vector_from_light(const Point& p) const
{
	return p - position;
}
