#include "Point.h"

Point::Point(const float vx, const float vy, const float vz)
	: Vector(vx, vy, vz)
{}

Point::Point(const Vector& p) : Vector(p) {}

Point& Point::operator=(const Vector& vec)
{
    Point tmp(vec);
    swap(*this, tmp);
    return *this;
}

Point& Point::operator=(const Point& p)
{
	Point temp(p);
	swap(*this, temp);
	return *this;
}
