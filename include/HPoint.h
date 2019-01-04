#pragma once
#ifndef HPOINT_H
#define HPOINT_H

class HVector;

class HPoint
{
public:
	HPoint();
	HPoint(float vx, float vy, float vz, float vw);
	HPoint(const HPoint &vec);
	HPoint& operator=(const HPoint &vec);
	~HPoint() = default;

	HPoint operator+(const HPoint &vec);
	HPoint& operator+=(const HPoint &vec);
	HPoint operator-(const HPoint &vec);
	HPoint& operator-=(const HPoint &vec);
	HPoint operator*(float value);
	HPoint& operator*=(float value);
	HPoint operator/(float value);
	HPoint& operator/=(float value);

	operator HVector() const;

	float dot(const HPoint &vec);
	void swap(HPoint& first, HPoint& second);

	float x;
	float y;
	float z;
	float w;
};

#endif