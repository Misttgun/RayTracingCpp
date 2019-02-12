#include "Cone.h"
#include <iostream>

bool Cone::intersect(const Ray& ray, Vector& impact) const
{
    float y_min = -1.0f;
    float y_max = 0.0f;

    Ray local_ray = global_to_local(ray);

    float a = pow(local_ray.direction[0], 2) - 
        pow(local_ray.direction[1], 2) + 
        pow(local_ray.direction[2], 2);

    float b = 2 * (local_ray.origin[0] * local_ray.direction[0] - 
        local_ray.origin[1] * local_ray.direction[1] + 
        local_ray.origin[2] * local_ray.direction[2]);

    float c = pow(local_ray.origin[0], 2) - 
        pow(local_ray.origin[1], 2) + 
        pow(local_ray.origin[2], 2);

    float t = -1.0f;

    if (!Entity::solve_polynomial_2(a, b, c, t))
        return false;

    const Vector local_impact = local_ray.origin + t * local_ray.direction;

    if (local_impact[1] < y_min || local_impact[1] > y_max)
        return false;

    impact = local_to_global_point(local_impact);

    return true;
}

Ray Cone::get_normal(const Vector& impact, const Vector& observator) const
{
    Vector local_impact = global_to_local_point(impact);
    Vector local_obs = global_to_local_point(observator);
    Vector local_ray_direction = local_impact - local_obs;

    // cone formula : x² - y² + z² = 0
    // gradient is the vector of partial derivative with respect to all variables
    // Gradient of the unit cone surface = (2x, -2y, 2z);
    // magnitude = sqrt (x² + y² + z²)

    float magnitude = sqrt(pow(local_impact[0], 2) +
        pow(local_impact[1], 2) +
        pow(local_impact[2], 2));

    Vector local_normal((2 * local_impact[0]) / magnitude, 
        (-2 * local_impact[1]) / magnitude, 
        (2 * local_impact[2]) / magnitude);
    
    if (local_ray_direction.dot(local_normal.normalized()) < 0)
        return Ray(impact, local_to_global_vector(local_normal).normalized());

    return Ray(impact, local_to_global_vector(local_normal * -1).normalized());
}

Material Cone::get_material(const Vector& impact) const
{

    Vector local_impact = global_to_local_point(impact);
    float x = local_impact[0], y = local_impact[1], z = local_impact[2];

    if (y >= -0.5f)
    {
        if (x < 0 && z >= 0 || x >= 0 && z < 0)
            return _material;

        return _material2;
    }

    else
    {
        if (x < 0 && z >= 0 || x >= 0 && z < 0)
            return _material2;

        return _material;
    }
}