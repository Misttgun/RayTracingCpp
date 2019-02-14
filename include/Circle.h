#pragma once

#ifndef _CIRCLE_H_INCLUDED_
#define _CIRCLE_H_INCLUDED_

#include "Plan.h"

class Circle : public Plan
{
    public:
        Circle() = default;
        Circle(float x, float y, float z) : Plan(x, y, z) {};

        bool intersect(const Ray& ray, Vector& impact) const override;  
};

#endif