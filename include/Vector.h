#pragma once
#ifndef VECTOR_H
#define VECTOR_H

#include "Point.h"

class Vector : public Point
{
public:
	Vector();
	Vector(float vx, float vy, float vz);
	Vector(const Vector &vec);
	Vector& operator=(const Vector &vec);
	~Vector() = default;

	float norm();
	Vector normalized();
};

#endif 