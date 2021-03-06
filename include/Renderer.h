#pragma once

#ifndef _RENDERER_H_INCLUDED_
#define _RENDERER_H_INCLUDED_

#include "Scene.h"

class Renderer
{
public:
    Renderer() = default;

    Color get_impact_color(const Ray& ray, const Object& obj, const Vector& impact, const Scene& scene, int depth) const;
    float get_shadow_color(const Ray& ray, const Object& obj, const Vector& impact, const Scene& scene, int depth) const;
};

#endif