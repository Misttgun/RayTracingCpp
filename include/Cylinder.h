#pragma once

#ifndef _CYLINDER_H_INCLUDED_
#define _CYLINDER_H_INCLUDED_

#include "Object.h"

class Cylinder : public Object
{
    public:
        Cylinder() = default;
        Cylinder(float x, float y, float z) : Object(x, y, z) {};

        bool intersect(const Ray& ray, Vector& impact) const override;
        Ray get_normal(const Vector& impact, const Vector& observator) const override;
};

#endif