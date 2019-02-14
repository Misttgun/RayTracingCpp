#pragma once

#ifndef _SCENE_H_INCLUDED_
#define _SCENE_H_INCLUDED_

#include <string>
#include <vector>
#include <sstream>
#include <memory>

#include "Object.h"
#include "Camera.h"
#include "Ray.h"
#include "Color.h"
#include "Light.h"

class Renderer;

class Scene
{
public:
    Scene() = default;
    Scene(const Scene& Copy);
    explicit Scene(int v_image_size, unsigned int v_sampling_factor);
    ~Scene();

    void render() const;
    std::shared_ptr<Object> closer_intersected(const Ray& ray, Vector& impact) const;

    Color get_background() const
    {
        return _background;
    }

    Color get_ambiant() const
    {
        return _ambiant;
    }

    int nb_lights() const
    {
        return _lights.size();
    }

    int nb_objects() const
    {
        return _objects.size();
    }

    std::shared_ptr<Light> get_light(int index) const
    {
        return _lights[index];
    }

    std::shared_ptr<Object> get_object(int index) const
    {
        return _objects[index];
    }

    void add_light(const std::shared_ptr<Light>& light)
    {
        _lights.push_back(light);
    }

    void add_object(const std::shared_ptr<Object>& object)
    {
        _objects.push_back(object);
    }

    Camera get_camera() const
    {
        return _camera;
    }

    void set_camera(const Camera& c)
    {
        _camera = c;
    }

    void set_ambiant(const Color& c)
    {
        _ambiant = c;
    }

    void set_bg(const Color& c)
    {
        _background = c;
    }

    Color cast_ray(const Ray& ray, Vector& impact, const Renderer& renderer, int depth) const;
    float compute_distance(const Vector& a, const Vector& b) const;
    Color** get_final_image() const;
    Color get_final_pixel(int i, int j) const;

    Color** image;
    int image_size;
    std::string output_file;
    bool apply_shadows;
    unsigned int _sampling_factor = 2;

private:
    Camera _camera;
    Color _background;
    Color _ambiant;
    std::vector<std::shared_ptr<Light>> _lights;
    std::vector<std::shared_ptr<Object>> _objects;
    const int MAX_DEPTH = 5;
};

#endif 
