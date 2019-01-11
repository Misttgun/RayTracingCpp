#pragma once

#ifndef _PLAN_H_INCLUDED_
#define _PLAN_H_INCLUDED_

#include "Object.h"

class Plan : public Object
{
    public:
        Plan() = default;
        Plan(float x, float y, float z) : Object(x, y, z) {};

        bool intersect(const Ray& ray, Point& impact) const;
        virtual Ray get_normal(const Point& impact, const Point& observator) const;
};

#endif