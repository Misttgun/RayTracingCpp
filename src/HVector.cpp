#include "HVector.h"

#include<math.h>
#include<assert.h>

HVector::HVector()
	:HPoint()
{}

HVector::HVector(float vx, float vy, float vz, float vw)
	: HPoint(vx, vy, vz, vw)
{}

HVector::HVector(const HVector & vec)
{
	x = vec.x;
	y = vec.y;
	z = vec.z;
	w = vec.w;
}

HVector& HVector::operator=(const HVector & vec)
{
	HVector temp(vec);
	swap(*this, temp);
	return *this;
}

float HVector::norm()
{
	return sqrtf(x * x + y * y + z * z + w * w);
}

HVector HVector::normalized()
{
	assert(norm() != 0.0f);
	*this /= norm();
	return *this;
}
