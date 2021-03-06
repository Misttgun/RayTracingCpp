#include "Scene.h"
#include "Plan.h"
#include <fstream>
#include "Renderer.h"

Scene::Scene(int v_image_size, unsigned int v_sampling_factor) : image_size(v_image_size), _sampling_factor(v_sampling_factor)
{
    const int final_size = image_size * _sampling_factor;

    image = new Color*[final_size];
    for (int i = 0; i < final_size; i++)
        image[i] = new Color[final_size];

    res = new Color*[image_size];
    for (int i = 0; i < image_size; i++)
        res[i] = new Color[image_size];

}

Scene::~Scene()
{
    const int final_size = image_size * _sampling_factor;

    for (auto i = 0; i < final_size; i++)
        delete image[i];

    delete[] image;

    for (int i = 0; i < image_size; i++)
        delete res[i];

    delete[] res;
}

Scene::Scene(const Scene& copy)
{
    image_size = copy.image_size;
    _sampling_factor = copy._sampling_factor;
    const int final_size = image_size * _sampling_factor;

    _camera = copy._camera;
    _background = copy._background;
    _ambiant = copy._ambiant;

    _lights.clear();
    for (const auto& light : copy._lights)
        _lights.push_back(light);

    _objects.clear();
    for (const auto& object : copy._objects)
        _objects.push_back(object);

    image = new Color*[final_size];
    for (int i = 0; i < final_size; i++)
        image[i] = new Color[final_size];

    for (int i = 0; i < final_size; i++)
        for (int j = 0; j < final_size; j++)
            image[i][j] = copy.image[i][j];

    res = new Color*[image_size];
    for (int i = 0; i < image_size; i++)
        res[i] = new Color[image_size];

    for (int i = 0; i < image_size; i++)
        for (int j = 0; j < image_size; j++)
            res[i][j] = copy.res[i][j];
}

std::shared_ptr<Object> Scene::closer_intersected(const Ray& ray, Vector& impact) const
{
    float current_distance = INFINITY;

    Vector tmp_impact;
    std::shared_ptr<Object> tmp_obj = nullptr;

    for (const auto& object : _objects)
    {
        // Pas d'intersection
        if (!object->intersect(ray, tmp_impact))
            continue;

        const float impact_distance = compute_distance(ray.origin, tmp_impact);

        // Un autre object est deja plus proche
        if (impact_distance >= current_distance)
            continue;

        // On met a jour le pointeur et la distance
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

    auto res_color = renderer.get_impact_color(ray, *intersected, impact, *this, depth);

    res_color.r = std::fmin(res_color.r, 1.0f);
    res_color.g = std::fmin(res_color.g, 1.0f);
    res_color.b = std::fmin(res_color.b, 1.0f);

    return  res_color;
}

Color** Scene::get_final_image() const
{
    for (int i = 0; i < image_size * _sampling_factor; i += _sampling_factor)
        for (int j = 0; j < image_size *_sampling_factor; j += _sampling_factor)
        {
            const int y = i / _sampling_factor, x = j / _sampling_factor;
            res[y][x] = get_final_pixel(i, j);
        }
    return res;
}

Color Scene::get_final_pixel(int y, int x) const
{
    Color res = image[y][x];
    int count = 1;

    for (int i = 0, y_offset = -_sampling_factor / 2; i < _sampling_factor; i++, y_offset++)
    {
        for (int j = 0, x_offset = -_sampling_factor / 2; j < _sampling_factor; j++)
        {
            if (x + x_offset < 0 || y + y_offset < 0 || (x_offset == 0 && y_offset == 0))
                continue;

            res += image[y + y_offset][x + x_offset];
            count++;
        }
    }

    return res / count;
}

float Scene::compute_distance(const Vector& a, const Vector& b) const
{
    return sqrt(pow(b[0] - a[0], 2) + pow(b[1] - a[1], 2) + pow(b[2] - a[2], 2));
}
