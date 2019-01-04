#include "Vector.h"

#include <cmath>
#include <cassert>
#include <utility>

Point::Point()
	:x(0.0f), y(0.0f), z(0.0f)
{}

Point::Point(float vx, float vy, float vz)
	: x(vx), y(vy), z(vz)
{}

Point::Point(const Point & vec)
{
	x = vec.x;
	y = vec.y;
	z = vec.z;
}

Point& Point::operator=(const Point & vec)
{
	Point temp(vec);
	swap(*this, temp);
	return *this;
}

Point Point::operator+(const Point & vec)
{
	return Point(x + vec.x, y + vec.y, z + vec.z);
}

Point& Point::operator+=(const Point & vec)
{
	x += vec.x;
	y += vec.y;
	z += vec.z;
	return *this;
}

Point Point::operator-(const Point & vec)
{
	return Point(x - vec.x, y - vec.y, z - vec.z);
}

Point& Point::operator-=(const Point & vec)
{
	x -= vec.x;
	y -= vec.y;
	z -= vec.z;
	return *this;
}

Point Point::operator*(float value)
{
	return Point(x * value, y * value, z * value);
}

Point& Point::operator*=(float value)
{
	x *= value;
	y *= value;
	z *= value;
	return *this;
}

Point Point::operator/(float value)
{
	assert(value != 0);
	return Point(x / value, y / value, z / value);
}

Point& Point::operator/=(float value)
{
	assert(value != 0);
	x /= value;
	y /= value;
	z /= value;
	return *this;
}

Point::operator Vector() const
{
	return Vector(x, y, z);
}

float Point::dot(const Point & vec)
{
	return x * vec.x + vec.y * y + vec.z * z;
}

void Point::swap(Point & first, Point & second)
{
	std::swap(first.x, second.x);
	std::swap(first.y, second.y);
	std::swap(first.z, second.z);
}
