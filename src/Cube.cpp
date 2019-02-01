#include "Cube.h"

bool Cube::intersect(const Ray& ray, Vector &impact) const
{
    /*Ray local_ray = local_to_global(ray);

    float t, t1, t2, tnear = -1000.0f, tfar = 1000.0f;

    for (int i = 0; i < 3; ++i)
    {
        if (local_ray.direction[i] == 0)
            if (local_ray.origin[i] < -1 || local_ray.origin[i] > 1)
                return false;

        t1 = (-1 - local_ray.origin[i]) / local_ray.direction[i];
        t2 = (1 - local_ray.origin[i]) / local_ray.direction[i];

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

    impact = local_to_global_point(local_ray.origin + t * local_ray.direction);

    return true;*/

    Vector orig = global_to_local_point(ray.origin);
    Vector dir = global_to_local_vector(ray.direction).normalized();

    float t;

    float tmin = (-1 - orig[0]) / dir[0];
    float tmax = (1 - orig[0]) / dir[0];

    if(tmin > tmax)
        std::swap(tmin, tmax);

    float tymin = (-1 - orig[1]) / dir[1];
    float tymax = (1 - orig[1]) / dir[1];

    if(tymin > tymax)
        std::swap(tymin, tymax);

    if((tmin > tymax) || (tymin > tmax))
        return false;

    if(tymin > tmin)
        tmin = tymin;    if(tymax < tmax)
        tmax = tymax;

    float tzmin = (-1 - orig[2]) / dir[2];
    float tzmax = (1 - orig[2]) / dir[2];

    if(tzmin > tzmax)
        std::swap(tzmin, tzmax);

    if((tmin > tzmax) || (tzmin > tmax))
        return false;

    if(tzmin > tmin)
        tmin = tzmin;

    if(tzmax < tmax)
        tmax = tzmax;

    t = tmin;

    if(t < 0)
    {
        t = tmax;
        if(t < 0) return false;
    }

    impact = local_to_global_point(orig) + local_to_global_vector(t * dir);

    return true;
}

Ray Cube::get_normal(const Vector& impact, const Vector& observator) const
{
    //Vector local_obs = global_to_local_point(observator);
    //Vector local_impact = global_to_local_point(impact);

    //Vector local_ray_direction(local_impact - local_obs);
    //local_ray_direction = local_ray_direction.normalized();

    //// - l'origine se trouve au centre 
    //Vector normal_origin(0, local_impact[1], 0);
    //Vector local_normal(local_impact);

    //if(local_impact[0] < -0.9999 && local_impact[0] > -1.0001)
    //    local_normal[0] = 2;

    //else if(local_impact[0] > 0.9999 && local_impact[0] < 1.0001)
    //    local_normal[0] = 0;

    //else if(local_impact[1] < -0.9999 && local_impact[1] > -1.0001)
    //    local_normal[1] = 0;

    //else if(local_impact[1] > 0.9999 && local_impact[1] < 1.0001)
    //    local_normal[1] = 2;

    //else if(local_impact[2] < -0.9999 && local_impact[2] > -1.0001)
    //    local_normal[2] = 0;

    //else if(local_impact[2] > 0.9999 && local_impact[2] < 1.0001)
    //    local_normal[2] = 2;

    //Vector local_normal_direction(local_normal - local_impact);
    //local_normal_direction = local_normal_direction.normalized();

    //if(local_ray_direction.dot(local_normal_direction) < 0)
    //    return Ray(impact, local_to_global_vector(local_normal_direction).normalized());

    //return Ray(impact, local_to_global_vector(local_normal_direction * -1).normalized());

    Vector nImpact = global_to_local_point(impact);
    Vector nObservator = global_to_local_point(observator);
    Vector normal(0, 0, 0);
    Vector dir = nImpact - nObservator;
    dir = dir.normalized();

    if(std::abs(nImpact[0] - 1.0f) < 0.0001f)
    {
        normal = Vector(1, 0, 0);
    }
    else if(std::abs(nImpact[1] - 1.0f) < 0.0001f)
    {
        normal = Vector(0, 1, 0);
    }
    else if(std::abs(nImpact[2] - 1.0f) < 0.0001f)
    {
        normal = Vector(0, 0, 1);
    }
    else if(std::abs(nImpact[0] + 1.0f) < 0.0001f)
    {
        normal = Vector(-1, 0, 0);
    }
    else if(std::abs(nImpact[1] + 1.0f) < 0.0001f)
    {
        normal = Vector(0, -1, 0);
    }
    else if(std::abs(nImpact[2] + 1.0f) < 0.0001f)
    {
        normal = Vector(0, 0, -1);
    }

    auto res = dir.dot(normal);
    if(res > 0)
        normal = normal * -1;

    normal = local_to_global_vector(normal);
    normal = normal.normalized();

    return Ray(impact, normal);
}