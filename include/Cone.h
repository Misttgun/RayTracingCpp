#pragma once

#ifndef _CONE_H_INCLUDED_
#define CONE_H_INCLUDED_

#include "Object.h"

class Cone : public Object
{
    public:
        Cone() = default;
        Cone(float x, float y, float z) : Object(x, y, z) {};

        bool intersect(const Ray& ray, Point& impact) const;
        Ray get_normal(const Point& impact, const Point& observator) const;
};

#endif