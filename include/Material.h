#pragma once
#ifndef MATERIAL_H
#define MATERIAL_H

#include <utility>
#include "Color.h"

class Material
{
public:
	Material() = default;
	Material(const Color& v_ka, const Color& v_kd, const Color& v_ks, float v_shininess);
	Material(const Material& c);
	~Material() = default;

	Material& operator= (const Material& rhs);

	friend void swap(Material& first, Material& second) noexcept
	{
		using std::swap;
		swap(first.ka, second.ka);
		swap(first.kd, second.kd);
		swap(first.ks, second.ks);
	}

	Color ka;
	Color kd;
	Color ks;
	float shininess;
};
#endif