#include "Scene.h"
#include "Plan.h"
#include "Square.h"
#include "Sphere.h"
#include "Cone.h"
#include "Cube.h"
#include "Cylinder.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include "Renderer.h"

Scene::Scene(int v_image_size) : image_size(v_image_size)
{
    image = new Color*[image_size];
    for (int i = 0; i < image_size; i++)
        image[i] = new Color[image_size];
}

Scene::~Scene()
{
    for (auto i = 0; i < image_size; i++)
        delete image[i];

    delete[] image;
}

Scene::Scene(const Scene& copy) : _lights(), _objects()
{
    image_size = copy.image_size;

    Camera _camera = copy._camera;
    Color _background = copy._background;
    Color _ambiant = copy._ambiant;

    for (auto light : copy._lights)
        _lights.push_back(light);

    for (auto object : copy._objects)
        _objects.push_back(object);

    image = new Color*[image_size];
    for (int i = 0; i < image_size; i++)
        image[i] = new Color[image_size];

    for (int i = 0; i < image_size; i++)
        for (int j = 0; j < image_size; j++)
            image[i][j] = copy.image[i][j];

    const int MAX_DEPTH = 5;
}

void Scene::render() const
{}

std::shared_ptr<Object> Scene::closer_intersected(const Ray& ray, Vector& impact) const
{
    float t = -1.f;
    float current_distance = INFINITY;

    Vector tmp_impact;
    std::shared_ptr<Object> tmp_obj = nullptr;

    for (auto object : _objects)
    {
        // - pas d'intersection
        if (!object->intersect(ray, tmp_impact))
            continue;

        float impact_distance = compute_distance(ray.origin, tmp_impact);

        // - un autre object est d�j� plus proche
        if (impact_distance >= current_distance)
            continue;

        // - on met � jour le pointeur et la distance
        current_distance = impact_distance;
        tmp_obj = object;
        impact = tmp_impact;
    }

    return tmp_obj;
}

Color Scene::cast_ray(const Ray& ray, Vector& impact, const Renderer& renderer, int depth) const
{
    if (depth >= MAX_DEPTH)
        return get_background();

    const std::shared_ptr<Object> intersected = closer_intersected(ray, impact);

    if (intersected == nullptr)
    {
        return get_background();
    }

    auto res_color = renderer.get_impact_color(ray, *intersected, impact, *this, depth) * renderer.get_shadow_color(ray, *intersected, impact, *this, depth);

    res_color.r = std::fmin(res_color.r, 1);
    res_color.g = std::fmin(res_color.g, 1);
    res_color.b = std::fmin(res_color.b, 1);

    return  res_color;
}

float Scene::compute_distance(const Vector& a, const Vector& b) const
{
    return sqrt(pow(b[0] - a[0], 2) + pow(b[1] - a[1], 2) + pow(b[2] - a[2], 2));
}
