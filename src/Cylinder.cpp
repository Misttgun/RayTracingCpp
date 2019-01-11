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

        // - le rayon est dirig� � l'oppos� du cylindre
        if (t1 < 0 && t2 < 0)
            return false;

        // - le rayon est dirig� vers le cylindre et a deux intersections
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