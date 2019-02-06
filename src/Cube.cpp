#include "Cube.h"

bool Cube::intersect(const Ray& ray, Vector &impact) const
{
    Vector orig = global_to_local_point(ray.origin);
    Vector dir = global_to_local_vector(ray.direction).normalized();

    float t;

    float tmin = (-1 - orig[0]) / dir[0];
    float tmax = (1 - orig[0]) / dir[0];

    if (tmin > tmax)
        std::swap(tmin, tmax);

    float tymin = (-1 - orig[1]) / dir[1];
    float tymax = (1 - orig[1]) / dir[1];

    if (tymin > tymax)
        std::swap(tymin, tymax);

    if ((tmin > tymax) || (tymin > tmax))
        return false;

    if (tymin > tmin)
        tmin = tymin;    if (tymax < tmax)
        tmax = tymax;

    float tzmin = (-1 - orig[2]) / dir[2];
    float tzmax = (1 - orig[2]) / dir[2];

    if (tzmin > tzmax)
        std::swap(tzmin, tzmax);

    if ((tmin > tzmax) || (tzmin > tmax))
        return false;

    if (tzmin > tmin)
        tmin = tzmin;

    if (tzmax < tmax)
        tmax = tzmax;

    t = tmin;

    if (t < 0)
    {
        t = tmax;
        if (t < 0) return false;
    }

    impact = local_to_global_point(orig) + local_to_global_vector(t * dir);

    return true;
}

Ray Cube::get_normal(const Vector& impact, const Vector& observator) const
{
    Vector nImpact = global_to_local_point(impact);
    Vector nObservator = global_to_local_point(observator);
    Vector normal(0, 0, 0);
    Vector dir = nImpact - nObservator;
    dir = dir.normalized();

    if (std::abs(nImpact[0] - 1.0f) < 0.0001f)
    {
        normal = Vector(1, 0, 0);
    }
    else if (std::abs(nImpact[1] - 1.0f) < 0.0001f)
    {
        normal = Vector(0, 1, 0);
    }
    else if (std::abs(nImpact[2] - 1.0f) < 0.0001f)
    {
        normal = Vector(0, 0, 1);
    }
    else if (std::abs(nImpact[0] + 1.0f) < 0.0001f)
    {
        normal = Vector(-1, 0, 0);
    }
    else if (std::abs(nImpact[1] + 1.0f) < 0.0001f)
    {
        normal = Vector(0, -1, 0);
    }
    else if (std::abs(nImpact[2] + 1.0f) < 0.0001f)
    {
        normal = Vector(0, 0, -1);
    }

    auto res = dir.dot(normal);
    if (res > 0)
        normal = normal * -1;

    normal = local_to_global_vector(normal);
    normal = normal.normalized();

    return Ray(impact, normal);
}