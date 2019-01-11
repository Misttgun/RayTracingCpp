#include "Plan.h"

Plan::Plan(float x, float y, float z) : Entity(x, y, z) { }

bool Plan::intersect (const Ray& ray, Point& impact) const
{
    Ray local_ray = global_to_local(ray);
    Point origin_local_plan(0, 0, 0);
    Point normal_local_plan(0, 0, 1);

    float dot_product = normal_local_plan.dot(local_ray.direction);

    // - rayon parall�le au plan
    if (dot_product == 0)
        return false;

    // - calcul du point d'impact
    float d = normal_local_plan.dot(local_ray.origin);
    float t = -d / dot_product;

    // - vecteur dirig� plan
    if (t > 0)
    {
        impact = local_to_global(local_ray.origin + t * local_ray.direction);
        return true;
    }

    // - vecteur dirig� � l'oppos� du plan
    return false;
}