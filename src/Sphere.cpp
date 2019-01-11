#include "Sphere.h"

Sphere::Sphere(float x, float y, float z) : Entity(x, y, z) {}

bool Sphere::intersect(const Ray& ray, Point& impact) const
{
    Ray local_ray = global_to_local(ray);

    float origin_squared = local_ray.origin.dot(local_ray.origin);
    float direction_squared = local_ray.direction.dot(local_ray.direction);
    float d = local_ray.direction.dot(local_ray.origin);
    float t = -1;

    float delta = (d * d) - direction_squared * (origin_squared - 1);

    // - pas d'intersection
    if (delta < 0)
        return false;

    // - un seul point d'intersection
    if (origin_squared < 1)
        t = (-d + sqrt(delta)) / direction_squared;

    else
    {
        if (d >= 0)
            return false;

        float t1 = (-d - sqrt(delta)) / direction_squared;
        float t2 = (-d + sqrt(delta)) / direction_squared;

        // - deux points d'intersection en direction de la sphère, on prend
        // le plus proche
        if (t1 > 0 && t2 > 0)
            t = t1 < t2 ? t1 : t2;

        // - t2 est le seul point d'intersection en direction de la sphère
        else if (t2 > 0)
            t = t2;

        // - t1 est le seul point d'intersection en direction de la sphère
        else
            t = t1;
    }

    impact = local_to_global(local_ray.origin + t * local_ray.direction);

    return true;

}