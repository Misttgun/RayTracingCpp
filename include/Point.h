#pragma once
#ifndef POINT_H
#define POINT_H

class Vector;

class Point
{
public:
	Point();
	Point(float vx, float vy, float vz);
	Point(const Point &vec);
	Point& operator=(const Point &vec);
	~Point() = default;

	Point operator+(const Point &vec);
	Point& operator+=(const Point &vec);
	Point operator-(const Point &vec);
	Point& operator-=(const Point &vec);
	Point operator*(float value);
	Point& operator*=(float value);
	Point operator/(float value);
	Point& operator/=(float value);

	operator Vector() const;

	float dot(const Point &vec);
	void swap(Point& first, Point& second);

	float x;
	float y;
	float z;
};

#endif