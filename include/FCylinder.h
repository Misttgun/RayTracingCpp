#pragma once

#ifndef _FCYLINDER_H_INCLUDED_
#define _FCYLINDER_H_INCLUDED_

#include "Cylinder.h"

class FCylinder : public Cylinder
{
public:
    FCylinder() = default;
    FCylinder(float x, float y, float z) : Cylinder(x, y, z)
    {};

    bool intersect(const Ray& ray, Vector& impact) const override;
};

#endif