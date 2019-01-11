#pragma once

#ifndef _SCENE_H_INCLUDED_
#define _SCENE_H_INCLUDED

#include <string>
#include <vector>

#include "Object.h"
#include "Camera.h"
#include "Ray.h"
#include "Point.h"
#include "Color.h"
#include "Matrix.h"
#include "Light.h"

class Scene
{
    public:
        Scene() = default;

        void render();
        void load(std::string file);
        std::shared_ptr<Object> closer_intersected(const Ray& ray, Point& impact) const;

        inline Color get_background() const { return _background; }
        inline Color get_ambiant() const { return _ambiant; }
        inline int nb_lights() const { return _lights.size(); }
        inline std::shared_ptr<Light> get_light(int index) const { return _lights[index]; }
        inline std::shared_ptr<Object> get_object(int index) const { return _objects[index]; }
        inline void add_light(std::shared_ptr<Light> light) { _lights.push_back(light); }
        inline void add_object(std::shared_ptr<Object> object) { _objects.push_back(object); }

        float compute_distance(const Point& a, const Point& b) const;

    private:
        Camera _camera;
        Color _background;
        Color _ambiant;
        std::vector<std::shared_ptr<Light>> _lights;
        std::vector<std::shared_ptr<Object>> _objects;
};

#endif 
