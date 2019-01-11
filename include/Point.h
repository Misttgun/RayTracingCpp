#pragma once
#ifndef POINT_H
#define POINT_H

#include "Vector.h"

class Point: public Vector
{
public:
	Point() = default;
	Point(float vx, float vy, float vz);
	Point(const Point& p) = default;
    Point(const Vector& p);
	Point& operator=(const Point& vec);
    Point& operator=(const Vector& vec);
	~Point() = default;

private:
	using Vector::norm;
	using Vector::normalized;
};

#endif