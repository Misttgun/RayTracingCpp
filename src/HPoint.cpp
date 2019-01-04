#include "HVector.h"

#include <cmath>
#include <cassert>
#include <utility>

HPoint::HPoint()
	:x(0.0f), y(0.0f), z(0.0f), w(0.0f)
{}

HPoint::HPoint(float vx, float vy, float vz, float vw)
	: x(vx), y(vy), z(vz), w(vw)
{}

HPoint::HPoint(const HPoint & vec)
{
	x = vec.x;
	y = vec.y;
	z = vec.z;
	w = vec.w;
}

HPoint& HPoint::operator=(const HPoint & vec)
{
	HPoint temp(vec);
	swap(*this, temp);
	return *this;
}

HPoint HPoint::operator+(const HPoint & vec)
{
	return HPoint(x + vec.x, y + vec.y, z + vec.z, w + vec.w);
}

HPoint& HPoint::operator+=(const HPoint & vec)
{
	x += vec.x;
	y += vec.y;
	z += vec.z;
	w += vec.w;
	return *this;
}

HPoint HPoint::operator-(const HPoint & vec)
{
	return HPoint(x - vec.x, y - vec.y, z - vec.z, w - vec.w);
}

HPoint& HPoint::operator-=(const HPoint & vec)
{
	x -= vec.x;
	y -= vec.y;
	z -= vec.z;
	w -= vec.w;
	return *this;
}

HPoint HPoint::operator*(float value)
{
	return HPoint(x * value, y * value, z * value, w * value);
}

HPoint& HPoint::operator*=(float value)
{
	x *= value;
	y *= value;
	z *= value;
	w *= value;
	return *this;
}

HPoint HPoint::operator/(float value)
{
	assert(value != 0);
	return HPoint(x / value, y / value, z / value, w / value);
}

HPoint& HPoint::operator/=(float value)
{
	assert(value != 0);
	x /= value;
	y /= value;
	z /= value;
	w /= value;
	return *this;
}

HPoint::operator HVector() const
{
	return HVector(x, y, z, w);
}

float HPoint::dot(const HPoint & vec)
{
	return x * vec.x + vec.y * y + vec.z * z + vec.w * w;
}

void HPoint::swap(HPoint & first, HPoint & second)
{
	std::swap(first.x, second.x);
	std::swap(first.y, second.y);
	std::swap(first.z, second.z);
	std::swap(first.w, second.w);
}
