#pragma once

#ifndef _SQUARE_H_INCLUDED_
#define _SQURE_H_INLCUDED

#include "Plan.h"

class Square : public Plan
{
    public:
        Square() = default;
        Square(float x, float y, float z);
        bool intersect(const Ray& ray, Point& impact) const override;
};

#endif
