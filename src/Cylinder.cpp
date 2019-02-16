#include "Cylinder.h"

bool Cylinder::intersect(const Ray& ray, Vector& impact) const
{
    Ray local_ray = global_to_local(ray);

    const float a = pow(local_ray.direction[0], 2) + pow(local_ray.direction[2], 2);
    const float b = 2 * (local_ray.origin[0] * local_ray.direction[0] + local_ray.origin[2] * local_ray.direction[2]);
    const float c = pow(local_ray.origin[0], 2) + pow(local_ray.origin[2], 2) - 1.f;

    float t = -1.0f;


    if (!solve_polynomial_2(a, b, c, t))
        return false;

    impact = local_to_global_point(local_ray.origin + t * local_ray.direction);

    return true;
}

Ray Cylinder::get_normal(const Vector& impact, const Vector& observator) const
{
    const Vector local_obs = global_to_local_point(observator);
    Vector local_impact = global_to_local_point(impact);

    Vector local_ray_direction(local_impact - local_obs);
    local_ray_direction = local_ray_direction.normalized();

    const Vector normal_origin(0, local_impact[1], 0);
    Vector local_normal(local_impact - normal_origin);
    local_normal = local_normal.normalized();

    if (local_ray_direction.dot(local_normal) < 0)
        return Ray(impact, local_to_global_vector(local_normal));

    return Ray(impact, local_to_global_vector(local_normal * -1));
}


Material Cylinder::get_material(const Vector& impact) const
{

    Vector local_impact = global_to_local_point(impact);
    const float x = local_impact[0];
    const int y = abs(static_cast<int>(floor(local_impact[1]))) % 2;
    const float z = local_impact[2];
    
    if (y == 0)
    {
        if (x < 0 && z >= 0 || x >= 0 && z < 0)
            return _material;

        return _material2;
    }

    if (x < 0 && z >= 0 || x >= 0 && z < 0)
        return _material2;

    return _material;
}
