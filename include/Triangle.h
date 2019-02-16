#pragma once

#ifndef _TRIANGLE_H_INCLUDED_
#define _TRIANGLE_H_INCLUDED_

#include "Plan.h"

class Triangle : public Plan
{
    public:
        Triangle() = default;
        Triangle(float x, float y, float z) : Plan(x, y, z) {};

        bool intersect(const Ray& ray, Vector& impact) const override;
};

#endif