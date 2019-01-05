#include "HPoint.h"

HPoint::HPoint(float vx, float vy, float vz, float vw)
	: HVector(vx, vy, vz, vw)
{}

HPoint& HPoint::operator=(const HPoint & vec)
{
	HPoint temp(vec);
	swap(*this, temp);
	return *this;
}
