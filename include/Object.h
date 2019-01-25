#pragma once

#ifndef _OBJECT_H_INCLUDED_
#define _OBJECT_H_INLCUDED_

#include "Entity.h"
#include "Material.h"
#include <cmath>

class Object : public Entity
{
    public:
        Object() = default;
        Object(float x, float y, float z) : Entity(x, y, z) {}

        virtual bool intersect(const Ray& ray, Vector& impact) const = 0;
        virtual Ray get_normal(const Vector& impact, const Vector& observator) const = 0;
        inline void set_material(Material m) { _material = m; }
        inline Material get_material() const { return _material;  }
    
    private:
        Material _material;
};

#endif
