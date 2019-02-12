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
        inline void set_material(Material m) { _material = m; _material2 = m; }
	inline void set_materials(Material m1, Material m2) { _material = m1; _material2 = m2; }
	// TODO : REDEFINE IN ALL CHILDS AND MAKE VIRTUAL
	Material get_material() const { return _material; } 
        virtual Material get_material(const Vector& impact) const { return _material; }
    
    protected:
        Material _material;
	Material _material2;
};

#endif
