#pragma once

#ifndef _CYLINDER_H_INCLUDED_
#define _CYLINDER_H_INCLUDED_

#include "Entity.h"

class Cylinder : public Entity
{
    public:
        Cylinder() = default;
        Cylinder(float x, float y, float z);
        bool intersect(const Ray& ray, Point& impact) const;
        Ray get_normal(const Point& impact, const Point& observator) const;
};

#endif