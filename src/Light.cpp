#include "Light.h"

Light::Light(const float x, const float y, const float z)
    : Entity(x, y, z)
{}

Light::Light(const float x, const float y, const float z, const Color& v_color) : Entity(x, y, z), color(v_color)
{}

Light::Light(const Light & c) : Entity(c)
{
    color = c.color;
}

Light& Light::operator=(const Light& rhs)
{
    Light temp(rhs);
    swap(temp, *this);
    return *this;
}

Ray Light::get_ray_to_light(const Vector& p) const
{
    const Vector local_p = global_to_local_point(p);

    const Vector dir = -1 * p;
    return local_to_global(Ray(local_p, dir));
}

Ray Light::get_ray_from_light(const Vector& p) const
{
    const Vector local_p = global_to_local_point(p);
    const Vector& dir = local_p;

    return local_to_global(Ray(Vector(0, 0, 0), dir));
}

Vector Light::get_vector_to_light(const Vector& p) const
{
    const Vector local_p = global_to_local_point(p);

    return local_to_global_vector(-1 * local_p);
}

Vector Light::get_vector_from_light(const Vector& p) const
{
    return local_to_global_vector(p);
}
