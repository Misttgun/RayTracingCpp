#pragma once
#ifndef HVECTOR_H
#define HVECTOR_H

#include "Point.h"
#include <utility>

class HVector
{
public:
	HVector();
	HVector(float vx, float vy, float vz, float vw);
	HVector(const HVector &vec);
	HVector& operator=(const HVector &vec);
	~HVector() = default;

	HVector operator+(const HVector &vec) const;
	HVector& operator+=(const HVector &vec);
	HVector operator-(const HVector &vec) const;
	HVector& operator-=(const HVector &vec);
	HVector operator*(float value) const;
	HVector& operator*=(float value);
	HVector operator/(float value) const;
	HVector& operator/=(float value);

	float operator[](int i) const;
	float& operator[](int i);

	operator Point() const;
	operator Vector() const;

	float dot(const HVector &vec) const;
	float norm() const;
	HVector normalized();
	friend void swap(HVector& first, HVector& second) noexcept
	{
		std::swap(first.x, second.x);
		std::swap(first.y, second.y);
		std::swap(first.z, second.z);
		std::swap(first.w, second.w);
	}

	float x;
	float y;
	float z;
	float w;
};

#endif 