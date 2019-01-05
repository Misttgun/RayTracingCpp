#pragma once
#ifndef HPOINT_H
#define HPOINT_H

#include "HVector.h"

class HPoint: public HVector
{
public:
	HPoint() = default;
	HPoint(float vx, float vy, float vz, float vw);
	HPoint(const HPoint &vec) = default;
	HPoint& operator=(const HPoint &vec);
	~HPoint() = default;

private:
	using HVector::norm;
	using HVector::normalized;
};

#endif