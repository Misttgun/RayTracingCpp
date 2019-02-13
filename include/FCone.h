#pragma once

#ifndef _FCONE_H_INCLUDED_
#define _FCONE_H_INCLUDED_

#include "Cone.h"

class FCone : public Cone
{
public:
    FCone() = default;
    FCone(float x, float y, float z) : Cone(x, y, z) {};

    bool intersect(const Ray& ray, Vector& impact) const override;
};

#endif