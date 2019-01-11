#pragma once

#ifndef _PLAN_H_INCLUDED_
#define _PLAN_H_INCLUDED_

#include "Entity.h"
#include "Ray.h"
#include "Point.h"

class Plan : public Entity
{
    public:
        Plan() = default;
        Plan(float x, float y, float z);
        bool intersect(const Ray& ray, Point& impact) const;

    private:

};

#endif