#include "Cube.h"

Cube::Cube(float x, float y, float z) : Entity(x, y, z) {}

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