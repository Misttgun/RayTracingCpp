#pragma once

#ifndef _RENDERER_H_INCLUDED_
#define _RENDERER_H_INCLUDED_

#include "Scene.h"

class Renderer
{
public:
    Color get_impact_color(const Ray& ray, const Object& obj, const Vector& impact, const Scene& scene) const;
    float get_shadow_color(const Ray& ray, const Object& obj, const Vector& impact, const Scene& scene) const;
    void save_ppm(std::string file, Color** pixel_map, int width, int height) const;

    bool reflect;
};

#endif