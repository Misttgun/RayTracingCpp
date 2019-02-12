#include "Material.h"

Material::Material(const Color& v_color, const float v_ka, const float v_kd, const float v_ks, const float v_shininess, const float v_light_influence, Type type)
	: color(v_color), ka(v_ka), kd(v_kd), ks(v_ks), shininess(v_shininess), light_influence(v_light_influence), mat_type(type)
{}

Material::Material(const Material & c)
{
    color = c.color;
	ka = c.ka;
	kd = c.kd;
	ks = c.ks;
	shininess = c.shininess;
    light_influence = c.light_influence;
    mat_type = c.mat_type;
}

Material& Material::operator=(const Material & rhs)
{
	Material temp(rhs);
	swap(temp, *this);
	return *this;
}
