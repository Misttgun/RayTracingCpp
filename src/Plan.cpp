#include "Plan.h"

bool Plan::intersect(const Ray& ray, Vector& impact) const
{
    Vector pOri(0, 0, 0);
    Vector pDir(0, 0, -1);

    Vector ori = global_to_local_point(ray.origin);
    Vector dire = global_to_local_vector(ray.direction);

    float denom = dire.dot(pDir);

    if (denom > 0)
    {
	Vector res = pOri - ori;
	float t = res.dot(pDir) / denom;

	impact = local_to_global_point(ori + t * dire.normalized());
	return t >= 0;
    }

    return false;
}

Ray Plan::get_normal(const Vector& impact, const Vector& observator) const
{
    Vector local_impact = global_to_local_point(impact);
    Vector local_obs = global_to_local_point(observator);

    Vector local_direction(0, 0, 1);
    Vector local_ray_direction = (local_impact - local_obs);

    // - dirige la normale du bon côté
    if (local_direction.dot(local_ray_direction) > 0)
	local_direction.z = -1;

    return Ray(impact, local_to_global_vector(local_direction));
}

Material Plan::get_material(const Vector& impact) const
{
    Vector local_impact = global_to_local_point(impact);
    int x = abs(static_cast<int>(floor(local_impact[0]))) % 2;
    int y = abs(static_cast<int>(floor(local_impact[1]))) % 2;

    if (x == 0 && y == 1 || x == 1 && y == 0)
	return _material;

    return _material2;
}
