#include "Square.h"

Square::Square(float x, float y, float z) : Plan(x, y, z) {}

bool Square::intersect(const Ray& ray, Point &impact) const
{
	Point ori = global_to_local(ray.origin);
	Vector dire = global_to_local(ray.direction);

	if (dire[2] < 0)
	{
		float t = -ori[2] / dire[2];
		impact = ori + t * dire;

		if (impact[0] <= 1 && impact[0] >= -1 && impact[1] <= 1 && impact[1] >= -1)
		{
			impact = local_to_global(impact);
			return t >= 0;
		}
	}

	return false;
}