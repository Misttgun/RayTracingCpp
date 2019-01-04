#pragma once
#ifndef HVECTOR_H
#define HVECTOR_H

#include "HPoint.h"

class HVector : public HPoint
{
public:
	HVector();
	HVector(float vx, float vy, float vz, float vw);
	HVector(const HVector &vec);
	HVector& operator=(const HVector &vec);
	~HVector() = default;

	float norm();
	HVector normalized();
};

#endif 