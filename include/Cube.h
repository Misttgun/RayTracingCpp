#pragma once

#ifndef _CUBE_H_INCLUDED_
#define _CUBE_H_INCLUDED_

#include "Object.h"

class Cube : public Object
{
    public:
        Cube() = default;
        Cube(float x, float y, float z) : Object(x, y, z) {};

        bool intersect(const Ray& ray, Point& impact) const;
        Ray get_normal(const Point& impact, const Point& observator) const;
};

#endif 
