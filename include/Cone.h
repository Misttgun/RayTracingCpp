#pragma once

#ifndef _CONE_H_INCLUDED_
#define _CONE_H_INCLUDED_

#include "Object.h"

class Cone : public Object
{
    public:
        Cone() = default;
        Cone(float x, float y, float z) : Object(x, y, z) {};

        bool intersect(const Ray& ray, Vector& impact) const override;
        Ray get_normal(const Vector& impact, const Vector& observator) const override;
        Material get_material(const Vector& impact) const override;
};

#endif