#include "Circle.h"

bool Circle::intersect(const Ray& ray, Vector& impact) const
{
    if (!Plan::intersect(ray, impact))
        return false;

    Vector local_impact = global_to_local_point(impact);

   return pow(local_impact[0], 2.0f) + pow(local_impact[1], 2.0f) <= 1.0f;
}
