#pragma once

#ifndef _TORE_H_INCLUDED_
#define _TORE_H_INCLUDED_

#include "Object.h"

class Tore : public Object
{
    public:
        Tore() = default;
        Tore(float x, float y, float z) : Object (x, y, z) {};

        bool intersect(const Ray& ray, Vector& impact) const override;
        Ray get_normal(const Vector& impact, const Vector& observator) const;
	Material get_material(const Vector& impact) const override;
};

#endif
