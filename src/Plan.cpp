#include "Plan.h"
#include <iostream>

bool Plan::intersect (const Ray& ray, Point& impact) const
{
    Ray local_ray = global_to_local(ray);
    Point origin_local_plan(0, 0, 0);
    Point normal_local_plan(0, 0, 1);

    float dot_product = normal_local_plan.dot(local_ray.direction);

    // - rayon parallèle au plan
    if (dot_product > 0.f)
        return false;

    // - calcul du point d'impact
    float d = normal_local_plan.dot(local_ray.origin - origin_local_plan);
    float t = -d / dot_product;

    // - vecteur dirigé plan
    if (t > 0)
    {
        impact = local_to_global(local_ray.origin + t * local_ray.direction);
        return true;
    }

    // - vecteur dirigé à l'opposé du plan
    return false;
}

Ray Plan::get_normal(const Point& impact, const Point& observator) const
{
    Point local_impact = global_to_local(impact);
    Point local_obs = global_to_local(observator);

    Vector local_direction(0, 0, 1);
    Vector local_ray_direction = (local_impact - local_obs);

    // - dirige la normale du bon côté
    if (local_direction.dot(local_ray_direction) > 0)
        local_direction.z = -1;



    return Ray(impact, local_to_global(local_direction));
}