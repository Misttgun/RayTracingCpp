#include "Cylinder.h"

bool Cylinder::intersect(const Ray& ray, Point& impact) const
{
    Ray local_ray = global_to_local(ray);

    float a = pow(local_ray.direction[0], 2) + pow(local_ray.direction[2], 2);
    float b = 2 * (local_ray.origin[0] * local_ray.direction[0] + local_ray.origin[0] * local_ray.direction[0]);
    float c = pow(local_ray.origin[0], 2) + pow(local_ray.origin[2], 2) - 1;
    float delta = b * b - 4 * a* c;
    float t = -1;


    if (!Entity::solve_polynomial_2(a, b, c, t))
        return false;

    impact = local_to_global(local_ray.origin + t * local_ray.direction);

    return true;
}

Ray Cylinder::get_normal(const Point& impact, const Point& observator) const
{
    Point local_obs = global_to_local(observator);
    Point local_impact = global_to_local(impact);

    Vector local_ray_direction(local_impact - local_obs);
    local_ray_direction = local_ray_direction.normalized();

    Vector normal_origin(0, local_impact[1], 0);
    Vector local_normal(local_impact - normal_origin);
    local_normal = local_normal.normalized();

    if (local_ray_direction.dot(local_normal) < 0)
        return Ray(impact, local_to_global(local_normal));

    return Ray(impact, local_to_global(local_normal * -1));
}