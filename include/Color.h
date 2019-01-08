#pragma once
#ifndef COLOR_H
#define COLOR_H

#include <utility>

class Color
{
public:
	Color();
	Color(float value);
	Color(float vr, float vg, float vb);
	Color(const Color& c);
	~Color() = default;

	Color& operator= (const Color& rhs);
	Color operator+ (const Color& c) const;
	Color& operator+= (const Color& c);
	Color operator* (const float a) const;
	Color& operator*= (const float a);
	Color operator/ (const float a) const;
	Color& operator/= (const float a);
	Color operator* (const Color& c) const;
	bool operator== (const Color& c) const;

	friend void swap(Color& first, Color& second) noexcept
	{
		using std::swap;
		swap(first.r, second.r);
		swap(first.g, second.g);
		swap(first.b, second.b);
	}

	float r;
	float g;
	float b;
};
#endif
