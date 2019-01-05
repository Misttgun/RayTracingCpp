#include "Point.h"

Point::Point(float vx, float vy, float vz)
	: Vector(vx, vy, vz)
{}

Point& Point::operator=(const Point& p)
{
	Point temp(p);
	swap(*this, temp);
	return *this;
}
