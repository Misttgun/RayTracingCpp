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

Ray Light::get_ray_to_light(const Vector& p) const
{
    Vector local_p = global_to_local_point(p);

	Vector dir = -1 * p;
	return local_to_global(Ray(local_p, dir));
}

Ray Light::get_ray_from_light(const Vector& p) const
{
    Vector local_p = global_to_local_point(p);
	Vector dir = local_p;

	return local_to_global(Ray(Vector(0,0,0), dir));
}

Vector Light::get_vector_to_light(const Vector& p) const
{
    Vector local_p = global_to_local_point(p);

	return local_to_global_vector(-1 * local_p);
}

Vector Light::get_vector_from_light(const Vector& p) const
{
    Vector local_p = global_to_local_vector(p);

	return local_to_global_vector(p);
}
