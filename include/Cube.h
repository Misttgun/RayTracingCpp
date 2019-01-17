#pragma once

#ifndef _CUBE_H_INCLUDED_
#define _CUBE_H_INCLUDED_

#include "Object.h"

class Cube : public Object
{
    public:
        Cube() = default;
        Cube(float x, float y, float z) : Object(x, y, z) {};

        bool intersect(const Ray& ray, Vector& impact) const override;
        Ray get_normal(const Vector& impact, const Vector& observator) const override;
};

#endif 
