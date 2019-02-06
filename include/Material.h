#pragma once
#ifndef MATERIAL_H
#define MATERIAL_H

#include <utility>
#include "Color.h"

class Material
{
public:
	Material() = default;
	Material(const Color& v_color, const float v_ka, const float v_kd, const float v_ks, const float v_shininess, const float v_light_influence);
	Material(const Material& c);
	~Material() = default;

	Material& operator= (const Material& rhs);

	friend void swap(Material& first, Material& second) noexcept
	{
		using std::swap;
        swap(first.color, second.color);
		swap(first.ka, second.ka);
		swap(first.kd, second.kd);
		swap(first.ks, second.ks);
        swap(first.shininess, second.shininess);
        swap(first.light_influence, second.light_influence);
	}

    Color color;
    float ka;
	float kd;
	float ks;
	float shininess;
    float light_influence;
};
#endif