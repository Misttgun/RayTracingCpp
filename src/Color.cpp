#include "Color.h"
#include <algorithm>

Color::Color()
	:r(0.0f), g(0.0f), b(0.0f)
{}

Color::Color(const float value)
	: r(value), g(value), b(value)
{}

Color::Color(const float vr, const float vg, const float vb)
	: r(vr), g(vg), b(vb)
{}

Color::Color(const Color& c)
{
	r = c.r;
	g = c.g;
	b = c.b;
}


Color& Color::operator=(const Color& rhs)
{
	Color temp(rhs);
	swap(temp, *this);
	return *this;
}

Color Color::operator+(const Color& c) const
{
	Color res(r + c.r, g + c.g, b + c.b);

	return res;
}

Color& Color::operator+=(const Color& c)
{
	r += c.r;
	g += c.g;
	b += c.b;

	return *this;
}

Color Color::operator*(const float a) const
{
	return Color(r * a, g * a, b * a);
}

Color& Color::operator*=(const float a)
{
	r *= a;
	g *= a;
	b *= a;
	return *this;
}

Color Color::operator/(const float a) const
{
	return Color(r / a, g / a, b / a);
}

Color& Color::operator/=(const float a)
{
	r /= a;
	g /= a;
	b /= a;
	return *this;
}

Color Color::operator*(const Color& c) const
{
	return Color(r * c.r, g * c.g, b * c.b);
}

bool Color::operator==(const Color& c) const
{
	return (r == c.r && g == c.g && b == c.b);
}

inline void Color::clamp()
{
	const float max_val = std::max(r, std::max(g, b));
	if (max_val > 1.0f)
		*this /= max_val;
}
