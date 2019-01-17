#pragma once

#ifndef _PLAN_H_INCLUDED_
#define _PLAN_H_INCLUDED_

#include "Object.h"

class Plan : public Object
{
public:
	Plan() = default;
	Plan(float x, float y, float z) : Object(x, y, z) {};

	bool intersect(const Ray& ray, Vector& impact) const override;
	Ray get_normal(const Vector& impact, const Vector& observator) const override;
};

#endif