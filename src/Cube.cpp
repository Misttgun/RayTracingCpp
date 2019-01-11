#include "Cube.h"

bool Cube::intersect(const Ray& ray, Point &impact) const
{
    Ray local_ray = local_to_global(ray);

    float t, t1, t2, tnear = -1000.0f, tfar = 1000.0f;

    for (int i = 0; i < 3; ++i)
    {
        if (local_ray.direction[i] == 0)
            if (local_ray.origin[i] < -1 || local_ray.origin[i] > 1)
                return false;

        t1 = (-1 - local_ray.origin[i]) / local_ray.direction[i];
        t2 = (-1 - local_ray.origin[i]) / local_ray.direction[i];

        if (t1 > t2)
            std::swap(t1, t2);

        if (t1 > tnear)
            tnear = t1;

        if (t2 < tfar)
            tfar = t2;

        if (tnear > tfar)
            return false;

        if (tfar < 0)
            return false;
    }

    t = tnear;

    impact = local_to_global(local_ray.origin + t * local_ray.direction);
    
    return true;
}

Ray Cube::get_normal(const Point& impact, const Point& observator) const
{
    Point local_obs = global_to_local(observator);
    Point local_impact = global_to_local(impact);

    Vector local_ray_direction(local_impact - local_obs);
    local_ray_direction = local_ray_direction.normalized();

    // - l'origine se trouve au centre 
    Vector normal_origin(0, local_impact[1], 0);
    Point local_normal(local_impact);

    if (local_impact[0] < -0.9999 && local_impact[0] > -1.0001)
        local_normal[0] = 2;

    else if (local_impact[0] > 0.9999 && local_impact[0] < 1.0001)
        local_normal[0] = 0;

    else if (local_impact[1] < -0.9999 && local_impact[1] > -1.0001)
        local_normal[1] = 0;

    else if (local_impact[1] > 0.9999 && local_impact[1] < 1.0001)
        local_normal[1] = 2;

    else if (local_impact[2] < -0.9999 && local_impact[2] > -1.0001)
        local_normal[2] = 0;

    else if (local_impact[2] > 0.9999 && local_impact[2] < 1.0001)
        local_normal[2] = 2;

    Vector local_normal_direction(local_normal - local_impact);
    local_normal_direction = local_normal_direction.normalized();

    if (local_ray_direction.dot(local_normal_direction) < 0)
        return Ray(impact, local_to_global(local_normal_direction).normalized());

    return Ray(impact, local_to_global(local_normal_direction * -1).normalized());
}