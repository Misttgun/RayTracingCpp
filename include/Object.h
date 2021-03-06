#pragma once

#ifndef _OBJECT_H_INCLUDED_
#define _OBJECT_H_INCLUDED_

#include "Entity.h"
#include "Material.h"

class Object : public Entity
{
public:
    Object() = default;
    Object(float x, float y, float z) : Entity(x, y, z)
    {}

    virtual bool intersect(const Ray& ray, Vector& impact) const = 0;
    virtual Ray get_normal(const Vector& impact, const Vector& observator) const = 0;

    void set_material(const Material& m)
    {
        _material = m; _material2 = m;
    }

    void set_materials(const Material& m1, const Material& m2)
    {
        _material = m1; _material2 = m2;
    }
    // TODO : REDEFINE IN ALL CHILDS AND MAKE VIRTUAL
    Material get_material() const
    {
        return _material;
    }
    virtual Material get_material(const Vector& impact) const
    {
        return _material;
    }

protected:
    Material _material;
    Material _material2;
};

#endif
