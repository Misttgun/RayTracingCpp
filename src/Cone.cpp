#include "Cone.h"
#include <iostream>

bool Cone::intersect(const Ray& ray, Vector& impact) const
{

    Ray local_ray = global_to_local(ray);

    const float a = pow(local_ray.direction[0], 2) - 
        pow(local_ray.direction[1], 2) + 
        pow(local_ray.direction[2], 2);

    const float b = 2 * (local_ray.origin[0] * local_ray.direction[0] - 
        local_ray.origin[1] * local_ray.direction[1] + 
        local_ray.origin[2] * local_ray.direction[2]);

    const float c = pow(local_ray.origin[0], 2) - 
        pow(local_ray.origin[1], 2) + 
        pow(local_ray.origin[2], 2);

    float t = -1.0f;

    if (!solve_polynomial_2(a, b, c, t))
        return false;

    const Vector local_impact = local_ray.origin + t * local_ray.direction;

    if (local_impact[1] > 0.0f)
        return false;

    impact = local_to_global_point(local_impact);

    return true;
}

Ray Cone::get_normal(const Vector& impact, const Vector& observator) const
{
    Vector local_impact = global_to_local_point(impact);
    const Vector local_obs = global_to_local_point(observator);
    Vector local_ray_direction = local_impact - local_obs;

    // cone formula : x� - y� + z� = 0
    // gradient is the vector of partial derivative with respect to all variables
    // Gradient of the unit cone surface = (2x, -2y, 2z);
    // magnitude = sqrt (x� + y� + z�)

    const float magnitude = sqrt(pow(local_impact[0], 2) +
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
    const float x = local_impact[0];
    const int y = abs(static_cast<int>(floor(local_impact[1] * 2))) % 2;
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
