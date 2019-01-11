#include "Scene.h"

void Scene::render()
{

}

void Scene::load(std::string file)
{

}

std::shared_ptr<Object> Scene::closer_intersected(const Ray& ray, Point& impact) const
{
    float t = -1.f;
    float current_distance = INFINITY;

    Point tmp_impact;
    std::shared_ptr<Object> tmp_obj = nullptr;

    for (auto object : _objects)
    {
        // - pas d'intersection
        if (!object->intersect(ray, tmp_impact))
            continue;

        float impact_distance = compute_distance(ray.origin, tmp_impact);

        // - un autre object est déjà plus proche
        if (impact_distance >= current_distance)
            continue;

        // - on met à jour le pointeur et la distance
        current_distance = impact_distance;
        tmp_obj = object;
    }

    impact = tmp_impact;

    return tmp_obj;
}

float Scene::compute_distance(const Point& a, const Point& b) const
{
    return sqrt(pow(b[0] - a[0], 2) + pow(b[1] - a[1], 2) + pow(b[2] - a[2], 2));
}