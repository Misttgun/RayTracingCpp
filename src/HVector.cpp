#include "HVector.h"

#include <cmath>
#include <cassert>


HVector::HVector()
	:x(0.0f), y(0.0f), z(0.0f), w(0.0f)
{}

HVector::HVector(float vx, float vy, float vz, float vw)
	: x(vx), y(vy), z(vz), w(vw)
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

HVector HVector::operator+(const HVector & vec) const
{
	return HVector(x + vec.x, y + vec.y, z + vec.z, w + vec.w);
}

HVector& HVector::operator+=(const HVector & vec)
{
	x += vec.x;
	y += vec.y;
	z += vec.z;
	w += vec.w;
	return *this;
}

HVector HVector::operator-(const HVector & vec) const
{
	return HVector(x - vec.x, y - vec.y, z - vec.z, w - vec.w);
}

HVector& HVector::operator-=(const HVector & vec)
{
	x -= vec.x;
	y -= vec.y;
	z -= vec.z;
	w -= vec.w;
	return *this;
}

HVector HVector::operator*(float value) const
{
	return HVector(x * value, y * value, z * value, w * value);
}

HVector& HVector::operator*=(float value)
{
	x *= value;
	y *= value;
	z *= value;
	w *= value;
	return *this;
}

HVector HVector::operator/(float value) const
{
	assert(value != 0);
	return HVector(x / value, y / value, z / value, w / value);
}

HVector& HVector::operator/=(float value)
{
	assert(value != 0);
	x /= value;
	y /= value;
	z /= value;
	w /= value;
	return *this;
}

float HVector::operator[](int i) const
{
	assert(i < 4);
	if (i == 0)
		return x;
	if (i == 1)
		return y;
	if (i == 2)
		return z;

	return w;
}

float& HVector::operator[](int i)
{
	assert(i < 4);
	if (i == 0)
		return x;
	if (i == 1)
		return y;
	if (i == 2)
		return z;

	return w;
}

HVector::operator Point() const
{
	return Point(x, y, z);
}

HVector::operator Vector() const
{
	return Vector(x, y, z);
}

float HVector::dot(const HVector & vec) const
{
	return x * vec.x + vec.y * y + vec.z * z + vec.w * w;
}

float HVector::norm() const
{
	return sqrtf(x * x + y * y + z * z + w * w);
}

HVector HVector::normalized()
{
	assert(norm() != 0.0f);
	*this /= norm();
	return *this;
}
