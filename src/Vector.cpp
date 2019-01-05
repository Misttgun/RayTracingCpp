#include "Vector.h"

#include <cmath>
#include <cassert>

Vector::Vector()
	:x(0.0f), y(0.0f), z(0.0f)
{}

Vector::Vector(float vx, float vy, float vz)
	: x(vx), y(vy), z(vz)
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

Vector Vector::operator+(const Vector & vec) const
{
	return Vector(x + vec.x, y + vec.y, z + vec.z);
}

Vector& Vector::operator+=(const Vector & vec)
{
	x += vec.x;
	y += vec.y;
	z += vec.z;
	return *this;
}

Vector Vector::operator-(const Vector & vec) const
{
	return Vector(x - vec.x, y - vec.y, z - vec.z);
}

Vector& Vector::operator-=(const Vector & vec)
{
	x -= vec.x;
	y -= vec.y;
	z -= vec.z;
	return *this;
}

Vector Vector::operator*(float value) const
{
	return Vector(x * value, y * value, z * value);
}

Vector& Vector::operator*=(float value)
{
	x *= value;
	y *= value;
	z *= value;
	return *this;
}

Vector Vector::operator/(float value) const
{
	assert(value != 0);
	return Vector(x / value, y / value, z / value);
}

Vector& Vector::operator/=(float value)
{
	assert(value != 0);
	x /= value;
	y /= value;
	z /= value;
	return *this;
}

float Vector::operator[](int i) const
{
	assert(i < 3);
	if (i == 0)
		return x;
	if (i == 1)
		return y;

	return z;
}

float& Vector::operator[](int i)
{
	assert(i < 3);
	if (i == 0)
		return x;
	if (i == 1)
		return y;

	return z;
}

float Vector::dot(const Vector & vec) const
{
	return x * vec.x + vec.y * y + vec.z * z;
}

float Vector::norm() const
{
	return sqrtf(x * x + y * y + z * z);
}

Vector Vector::normalized()
{
	assert(norm() != 0.0f);
	*this /= norm();
	return *this;
}
