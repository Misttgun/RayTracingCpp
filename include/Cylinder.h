#pragma once

#ifndef _CYLINDER_H_INCLUDED_
#define _CYLINDER_H_INCLUDED_

#include "Object.h"

class Cylinder : public Object
{
    public:
        Cylinder() = default;
        Cylinder(float x, float y, float z) : Object(x, y, z) {};

        bool intersect(const Ray& ray, Point& impact) const;
        Ray get_normal(const Point& impact, const Point& observator) const;
};

#endif