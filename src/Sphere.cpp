#include "Sphere.h"

bool Sphere::intersect(const Ray& ray, Point& impact) const
{
	Point ori = global_to_local(ray.origin);
	Vector dir = global_to_local(ray.direction).normalized();

	float t0;
	float t1;
	float t;

	Point center(0, 0, 0);
	Vector L = center - ori;

	float tca = L.dot(dir);

	float d2 = L.dot(L) - tca * tca;
	if (d2 > 1) return false;

	float thc = sqrt(1 - d2);
	t0 = tca - thc;
	t1 = tca + thc;

	if (t0 > t1) std::swap(t0, t1);

	if (t0 < 0) {
		t0 = t1;
		if (t0 < 0) return false;
	}

	t = t0;

	impact = local_to_global(ori + t * dir);
	return true;
    //Ray local_ray = global_to_local(ray);

    //float origin_squared = local_ray.origin.dot(local_ray.origin);
    //float direction_squared = local_ray.direction.dot(local_ray.direction);
    //float d = local_ray.direction.dot(local_ray.origin);
    //float t = -1;

    //float delta = (d * d) - direction_squared * (origin_squared - 1);

    //// - pas d'intersection
    //if (delta < 0)
    //    return false;

    //// - un seul point d'intersection
    //if (origin_squared < 1)
    //    t = (-d + sqrt(delta)) / direction_squared;

    //else
    //{
    //    if (d >= 0)
    //        return false;

    //    float t1 = (-d - sqrt(delta)) / direction_squared;
    //    float t2 = (-d + sqrt(delta)) / direction_squared;

    //    // - deux points d'intersection en direction de la sphère, on prend
    //    // le plus proche
    //    if (t1 > 0 && t2 > 0)
    //        t = t1 < t2 ? t1 : t2;

    //    // - t2 est le seul point d'intersection en direction de la sphère
    //    else if (t2 > 0)
    //        t = t2;

    //    // - t1 est le seul point d'intersection en direction de la sphère
    //    else
    //        t = t1;
    //}

    //impact = local_to_global(local_ray.origin + t * local_ray.direction);

    //return true;

}

Ray Sphere::get_normal(const Point& impact, const Point& observator) const
{
    Point local_obs = global_to_local(observator);
    Point local_impact = global_to_local(impact);

    Vector local_ray_direction(local_impact - local_obs);
    Vector local_normal(local_impact);
    
    local_normal = local_normal.normalized();

    if (local_ray_direction.dot(local_normal) < 0)
        return Ray(impact, local_to_global(local_normal).normalized());

    return Ray(impact, local_to_global((local_normal * -1)).normalized());
}