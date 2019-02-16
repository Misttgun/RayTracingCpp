#include "FCone.h"

bool FCone::intersect(const Ray& ray, Vector& impact) const
{
    if (!Cone::intersect(ray, impact))
        return false;

    Vector local_impact = global_to_local_point(impact);

    return local_impact[1] > -1.0f;
}