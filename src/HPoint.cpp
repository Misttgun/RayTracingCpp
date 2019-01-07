#include "HPoint.h"

HPoint::HPoint(const float vx, const float vy, const float vz, const float vw)
	: HVector(vx, vy, vz, vw)
{}

HPoint& HPoint::operator=(const HPoint & vec)
{
	HPoint temp(vec);
	swap(*this, temp);
	return *this;
}
