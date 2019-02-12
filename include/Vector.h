#pragma once
#ifndef VECTOR_H
#define VECTOR_H

#include <utility>

class Vector
{
public:
	Vector();
	Vector(float vx, float vy, float vz);
	Vector(const Vector &vec);
	Vector& operator=(const Vector &vec);
	~Vector() = default;

	Vector operator+(const Vector &vec) const;
	Vector& operator+=(const Vector &vec);
	Vector operator-(const Vector &vec) const;
	Vector& operator-=(const Vector &vec);
	Vector operator*(float value) const;
	Vector& operator*=(float value);
	Vector operator/(float value) const;
	Vector& operator/=(float value);
    bool operator!= (const Vector& value);

	float operator[](int i) const;
	float& operator[](int i);

	float dot(const Vector &vec) const;
	float norm() const;
	Vector normalized();
	friend void swap(Vector& first, Vector& second) noexcept
	{
		using std::swap;
		swap(first.x, second.x);
		swap(first.y, second.y);
		swap(first.z, second.z);
	}

	float x;
	float y;
	float z;
};

Vector operator*(float a, const Vector& v);

#endif 