#pragma once

#ifndef _SPHERE_H_INCLUDED_
#define _SPHERE_H_INCLUDED_

#include "Entity.h"

class Sphere : public Entity
{
    public:
        Sphere() = default;
        Sphere(float x, float y, float z);
        bool intersect(const Ray& ray, Point& impact) const;
};

#endif