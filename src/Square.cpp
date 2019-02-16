#include "Square.h"

Square::Square(float x, float y, float z) : Plan(x, y, z) {}

bool Square::intersect(const Ray& ray, Vector &impact) const
{
    Vector ori = global_to_local_point(ray.origin);
	Vector dire = global_to_local_vector(ray.direction);

	if (dire[2] < 0)
	{
	    const float t = -ori[2] / dire[2];
		impact = ori + t * dire;

		if (impact[0] <= 1 && impact[0] >= -1 && impact[1] <= 1 && impact[1] >= -1)
		{
			impact = local_to_global_point(impact);
			return t >= 0;
		}
	}

	return false;
}