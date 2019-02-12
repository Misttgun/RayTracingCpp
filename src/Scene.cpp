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

void Scene::render() const
{}

void Scene::load(const std::string& file)
{
    std::string line;
    std::ifstream in(file.c_str());

    int line_counter = 0;
    int nb_obj;

    while (std::getline(in, line))
    {
        std::istringstream iss(line);

        if (line_counter == 0)
            load_globals(iss, nb_obj);

        else if (line_counter - 1 < nb_obj)
            load_object(iss);

        else
            load_light(iss);

        line_counter++;
    }


    in.close();
}

void Scene::load_globals(std::istringstream& params, int& nb_obj)
{
    float r, g, b, tr_x, tr_y, tr_z, focal;

    params >> tr_x;
    params >> tr_y;
    params >> tr_z;
    params >> focal;

    _camera = Camera(tr_x, tr_y, tr_z, focal);

    params >> r;
    params >> g;
    params >> b;

    _background = Color(r, g, b);

    params >> r;
    params >> g;
    params >> b;

    _ambiant = Color(r, g, b);

    params >> nb_obj;
}

void Scene::load_object(std::istringstream& params)
{
    char object;
    float tr_x, tr_y, tr_z, r_x, r_y, r_z, s, r, g, b, shininess;

    params >> object;

    // - translation
    params >> tr_x;
    params >> tr_y;
    params >> tr_z;

    // - rotations
    params >> r_x;
    params >> r_y;
    params >> r_z;

    // - scale
    params >> s;

    // - ka
    params >> r;
    params >> g;
    params >> b;
    Color ka = Color(r, g, b);

    // - kd
    params >> r;
    params >> g;
    params >> b;
    Color kd = Color(r, g, b);

    // - ks 
    params >> r;
    params >> g;
    params >> b;
    Color ks = Color(r, g, b);

    // - shininess
    params >> shininess;

    //Material mat(ka, kd, ks, shininess);
    std::shared_ptr<Object> o = nullptr;

    switch (object)
    {
        case 'P':
            o = std::make_shared<Plan>(Plan());
            break;
        case 'Q':
            o = std::make_shared<Square>(Square());
            break;
        case 'S':
            o = std::make_shared<Sphere>(Sphere());
            break;
        case 'C':
            o = std::make_shared<Cone>(Cone());
            break;
        case 'U':
            o = std::make_shared<Cube>(Cube());
            break;
        case 'Y':
            o = std::make_shared<Cylinder>(Cylinder());
            break;
        default:
            return;
    }

    if (tr_x > 0.00001f || tr_x < -0.00001f && tr_y > 0.00001f || tr_y < -0.00001f && tr_z > 0.00001f || tr_z < -0.00001f)
        o->translate(tr_x, tr_y, tr_z);

    if (r_x > 0.00001f || r_x < -0.00001f)
        o->rotate_x(r_x);

    if (r_y > 0.00001f || r_y < -0.00001f)
        o->rotate_y(r_y);

    if (r_z > 0.00001f || r_z < -0.00001f)
        o->rotate_z(r_z);
    o->scale(s);
    //o->set_material(mat);
    add_object(o);
}

void Scene::load_light(std::istringstream& params) const
{
    float tr_x, tr_y, tr_z, r, g, b;

    // - translation
    params >> tr_x;
    params >> tr_y;
    params >> tr_z;

    // - id
    params >> r;
    params >> g;
    params >> b;
    Color id = Color(r, g, b);

    // - is
    params >> r;
    params >> g;
    params >> b;
    Color is = Color(r, g, b);

    //std::shared_ptr<Light> light = std::make_shared<Light>(Light(tr_x, tr_y, tr_z, id, is));
    //add_light(light);
}

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

        // - un autre object est déjà plus proche
        if (impact_distance >= current_distance)
            continue;

        // - on met à jour le pointeur et la distance
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

    auto res_color = renderer.get_impact_color(ray, *intersected, impact, *this, depth)/* * renderer.get_shadow_color(ray, *intersected, impact, *this, depth)*/;

    res_color.r = std::fmin(res_color.r, 1);
    res_color.g = std::fmin(res_color.g, 1);
    res_color.b = std::fmin(res_color.b, 1);

    return  res_color;
}

float Scene::compute_distance(const Vector& a, const Vector& b) const
{
    return sqrt(pow(b[0] - a[0], 2) + pow(b[1] - a[1], 2) + pow(b[2] - a[2], 2));
}
