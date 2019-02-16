#pragma once

#ifndef _SQUARE_H_INCLUDED_
#define _SQUARE_H_INCLUDED_

#include "Plan.h"

class Square : public Plan
{
    public:
        Square() = default;
        Square(float x, float y, float z);
        bool intersect(const Ray& ray, Vector& impact) const override;
};

#endif
