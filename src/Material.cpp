#include "Material.h"

Material::Material(const Color& v_ka, const Color& v_kd, const Color& v_ks, const float v_shininess)
	: ka(v_ka), kd(v_kd), ks(v_ks), shininess(v_shininess)
{}

Material::Material(const Material & c)
{
	ka = c.ka;
	kd = c.kd;
	ks = c.ks;
	shininess = c.shininess;
}

Material& Material::operator=(const Material & rhs)
{
	Material temp(rhs);
	swap(temp, *this);
	return *this;
}
