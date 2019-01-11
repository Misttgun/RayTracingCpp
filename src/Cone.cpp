#include "Cone.h"

Cone::Cone(float x, float y, float z) : Entity(x, y, z) {}

bool Cone::intersect(const Ray& ray, Point& impact) const
{
    float y_min = 0;
    float y_max = 1;

    Ray local_ray = global_to_local(ray);

    float a = pow(local_ray.direction[0], 2) - pow(local_ray.direction[1], 2) + pow(local_ray.direction[2], 2);
    float b = 2 * (local_ray.origin[0] * local_ray.direction[0] - local_ray.origin[1] * local_ray.direction[1] + local_ray.origin[2] * local_ray.direction[2]);
    float c = pow(local_ray.origin[0], 2) - pow(local_ray.origin[1], 2) - pow(local_ray.origin[2], 2);
    float t = -1;

    if (!Entity::solve_polynomial_2(a, b, c, t))
        return false;

    Point local_impact = local_ray.origin + t * local_ray.direction;

    // - on teste que le point d'impact se trouve dans le cone simple
    // fini et ouvert
    if (local_impact[1] < y_min || local_impact[1] > y_max)
        return false;

    impact = local_to_global(local_impact);

    return true;
}

Ray Cone::get_normal(const Point& impact, const Point& observator) const
{
    // - on dispose du point d'impact et du sommet du cone (0, 0, 0)
    // à partir de ces deux informations on peut calculer un vecteur normal

    Point local_impact = global_to_local(impact);
    Point local_obs = global_to_local(observator);

    Vector local_ray_direction = Vector(local_impact - local_obs);
    Vector plan = Vector(local_impact);
    Vector local_normal;

    // - calcul un vecteur orthogonal a V(impact - origin)
    if (Entity::is_epsilon(0.f, local_impact[0], 0.0001f) && Entity::is_epsilon(0.f, local_impact[1], 0.0001f))
    {
        // - présent dans l'algorithme original, mais redondant dans le cas du cone : 
        // https://codereview.stackexchange.com/questions/43928/algorithm-to-get-an-arbitrary-perpendicular-vector
        /*
        if (Entity::is_epsilon(0.f, local_impact[2], 0.0001f))
            local_normal = Vector(0, 1, 0);
        */

        local_normal = Vector(0, 1, 0);
    }

    local_normal = Vector(-local_impact[1], local_impact[0], 0);

    local_normal = local_normal.normalized();

    if (local_ray_direction.dot(local_normal) < 0)
        return Ray(impact, local_to_global(local_normal));

    return Ray(impact, local_to_global(local_normal * -1));
}