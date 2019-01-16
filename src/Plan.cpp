#include "Plan.h"

bool Plan::intersect(const Ray& ray, Point& impact) const
{
	Point pOri(0, 0, 0);
	Vector pDir(0, 0, -1);

	Point ori = global_to_local(ray.origin);
	Vector dire = global_to_local(ray.direction);

	float denom = dire.dot(pDir);

	if (denom > 0)
	{
		Vector res = pOri - ori;
		float t = res.dot(pDir) / denom;
		impact = local_to_global(ori + t * dire);
		return t >= 0;
	}

	return false;
}

Ray Plan::get_normal(const Point& impact, const Point& observator) const
{
	Point local_impact = global_to_local(impact);
	Point local_obs = global_to_local(observator);

	Vector local_direction(0, 0, 1);
	Vector local_ray_direction = (local_impact - local_obs);

	// - dirige la normale du bon côté
	if (local_direction.dot(local_ray_direction) > 0)
		local_direction.z = -1;



	return Ray(impact, local_to_global(local_direction));
}