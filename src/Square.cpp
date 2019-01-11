#include "Square.h"

Square::Square(float x, float y, float z) : Plan(x, y, z) {}

bool Square::intersect(const Ray& ray, Point &impact) const
{
    // - test d'intersection
    bool is_intesecting = Plan::intersect(ray, impact);

    impact = global_to_local(impact);

    // - test d'appartenance
    is_intesecting &= (impact[0] >= -1 && impact[0] <= 1 && impact[1] >= -1 && impact[1] <= 1);

    impact = local_to_global(impact);

    return is_intesecting;
}