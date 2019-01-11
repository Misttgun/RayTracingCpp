#pragma once

#ifndef _CONE_H_INCLUDED_
#define CONE_H_INCLUDED_

#include "Entity.h"

class Cone : public Entity
{
    public:
        Cone() = default;
        Cone(float x, float y, float z);
        bool intersect(const Ray& ray, Point& impact) const;
        Ray get_normal(const Point& impact, const Point& observator) const;
};

#endif