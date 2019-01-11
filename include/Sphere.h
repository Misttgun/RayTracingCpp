#pragma once

#ifndef _SPHERE_H_INCLUDED_
#define _SPHERE_H_INCLUDED_

#include "Object.h"

class Sphere : public Object
{
    public:
        Sphere() = default;
        Sphere(float x, float y, float z) : Object (x, y, z) {};

        bool intersect(const Ray& ray, Point& impact) const;
        Ray get_normal(const Point& impact, const Point& observator) const;
};

#endif