#pragma once

#ifndef _OBJECT_H_INCLUDED_
#define _OBJECT_H_INLCUDED_

#include "Entity.h"

class Object : public Entity
{
public:
    Object() = default;
    Object(float x, float y, float z) : Entity(x, y, z) {}

    virtual bool intersect(const Ray& ray, Point& impact) const = 0;
    virtual Ray get_normal(const Point& impact, const Point& observator) const = 0;
};

#endif
