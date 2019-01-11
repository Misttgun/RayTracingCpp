#include "Cylinder.h"

Cylinder::Cylinder(float x, float y, float z) : Entity(x, y, z) {}

bool Cylinder::intersect(const Ray& ray, Point& impact) const
{
    Ray local_ray = global_to_local(ray);

    float a = pow(local_ray.direction[0], 2) + pow(local_ray.direction[2], 2);
    float b = 2 * (local_ray.origin[0] * local_ray.direction[0] + local_ray.origin[0] * local_ray.direction[0]);
    float c = pow(local_ray.origin[0], 2) + pow(local_ray.origin[2], 2) - 1;
    float delta = b * b - 4 * a* c;
    float t = -1;

    // - pas d'intersection
    if (delta < 0)
        return false;

    // - une intersection
    if (delta == 0)
        t = -b / (2 * a);

    // - deux intersections
    else
    {
        float t1 = (-b - sqrt(delta)) / (2 * a);
        float t2 = (-b + sqrt(delta)) / (2 * a);

        // - le rayon est dirigé à l'opposé du cylindre
        if (t1 < 0 && t2 < 0)
            return false;

        // - le rayon est dirigé vers le cylindre et a deux intersections
        if (t1 > 0 && t2 > 0)
            t = t1 < t2 ? t1 : t2;

        // - origine du rayon dans le cylindre et seul t1 visible
        else if (t1 > 0)
            t = t1;

        // - origine du rayon dans le cylindre et seul t2 visible
        else
            t = t2;
    }

    if (t < 0)
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