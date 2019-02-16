#include "Triangle.h"

bool Triangle::intersect(const Ray& ray, Vector& impact) const
{
    if (!Plan::intersect(ray, impact))
        return false;

    const Vector p1(-0.5f, -0.5f, 0), p2(0.5f, -0.5f, 0), p3(0, 0, 0);
    const Vector local_impact = global_to_local_point(impact);
    const Vector R = local_impact - p1;
    const Vector Q1 = p2 - p1;
    const Vector Q2 = p3 - p1;

    const float sq_Q1 = Q1.dot(Q1);
    const float sq_Q2 = Q2.dot(Q2);
    const float RQ1 = R.dot(Q1);
    const float RQ2 = R.dot(Q2);
    const float Q1Q2 = Q1.dot(Q2);
    const float factor = 1.0f / (sq_Q1*sq_Q2 - pow(Q1Q2, 2.0f));

    const float w1 = factor * (sq_Q2 * RQ1 - Q1Q2 * RQ2);
    const float w2 = factor * ((-Q1Q2 * RQ1) + sq_Q1 * RQ2);

    return (w1 + w2) <= 1.0f && w1 >= 0.f && w2 >= 0.f;
}