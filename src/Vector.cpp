#include "Vector.h"

#include<math.h>
#include<assert.h>

Vector::Vector()
	:Point()
{}

Vector::Vector(float vx, float vy, float vz)
	: Point(vx, vy, vz)
{}

Vector::Vector(const Vector & vec)
{
	x = vec.x;
	y = vec.y;
	z = vec.z;
}

Vector& Vector::operator=(const Vector & vec)
{
	Vector temp(vec);
	swap(*this, temp);
	return *this;
}

float Vector::norm()
{
	return sqrtf(x * x + y * y + z * z);
}

Vector Vector::normalized()
{
	assert(norm() != 0.0f);
	*this /= norm();
	return *this;
}
