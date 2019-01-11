#pragma once

#ifndef _CUBE_H_INCLUDED_
#define _CUBE_H_INCLUDED_

#include "Entity.h"

class Cube : public Entity
{
    public:
        Cube() = default;
        Cube(float x, float y, float z);
        bool intersect(const Ray& ray, Point& impact) const;
};

#endif 
