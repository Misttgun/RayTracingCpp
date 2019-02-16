#pragma once

#ifndef _SPHERE_H_INCLUDED_
#define _SPHERE_H_INCLUDED_

#include "Object.h"

class Sphere : public Object
{
public:
    Sphere() = default;
    Sphere(float x, float y, float z) : Object(x, y, z)
    {};

    bool intersect(const Ray& ray, Vector& impact) const override;
    Ray get_normal(const Vector& impact, const Vector& observator) const override;
    Material get_material(const Vector& impact) const override;
};

#endif
