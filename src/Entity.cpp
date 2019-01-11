#include "Entity.h"
#include "HPoint.h"
#include "HVector.h"

#include <cmath>

Entity::Entity(const float x, const float y, const float z)
	:position(x, y, z)
{}

Entity::Entity(const Entity & entity)
{
	position = entity.position;
	trans = entity.trans;
	transInv = entity.transInv;
}

Entity & Entity::operator=(const Entity & other)
{
	Entity temp(other);
	swap(*this, temp);
	return *this;
}

void Entity::translate(const float x, const float y, const float z)
{
	Matrix mat;
	mat(0, 3) = x;
	mat(1, 3) = y;
	mat(2, 3) = z;

	trans = mat * trans;
	transInv = trans.inverse();
}

void Entity::rotate_x(const float deg)
{
	Matrix mat;
	mat(1, 1) = cos(deg);
	mat(1, 2) = -sin(deg);
	mat(2, 1) = sin(deg);
	mat(2, 2) = cos(deg);

	trans = mat * trans;
	transInv = trans.inverse();
}

void Entity::rotate_y(const float deg)
{
	Matrix mat;
	mat(0, 0) = cos(deg);
	mat(0, 2) = sin(deg);
	mat(2, 0) = -sin(deg);
	mat(2, 2) = cos(deg);

	trans = mat * trans;
	transInv = trans.inverse();
}

void Entity::rotate_z(const float deg)
{
	Matrix mat;
	mat(0, 0) = cos(deg);
	mat(0, 1) = -sin(deg);
	mat(1, 0) = sin(deg);
	mat(1, 1) = cos(deg);

	trans = mat * trans;
	transInv = trans.inverse();
}

void Entity::scale(const float factor)
{
	Matrix mat;
	mat(0, 0) = factor;
	mat(1, 1) = factor;
	mat(2, 2) = factor;

	trans = mat * trans;
	transInv = trans.inverse();
}

Point Entity::local_to_global(const Point & p) const
{
	HPoint point(p.x, p.y, p.z, 1);
	return  transInv * point;
}

Vector Entity::local_to_global(const Vector & v) const
{
	HVector vector(v.x, v.y, v.z, 0);
	return  transInv * vector;
}

Ray Entity::local_to_global(const Ray & r) const
{
	Vector v = r.direction;
	Point p = r.origin;
	HPoint point(p.x, p.y, p.z, 1);
	HVector vector(v.x, v.y, v.z, 0);
	
	return Ray(transInv * point, transInv * vector);
}

Point Entity::global_to_local(const Point & p) const
{
	HPoint point(p.x, p.y, p.z, 1);
	return  trans * point;
}

Vector Entity::global_to_local(const Vector & v) const
{
	HVector vector(v.x, v.y, v.z, 0);
	return  trans * vector;
}

Ray Entity::global_to_local(const Ray& r) const
{
	Vector v = r.direction;
	Point p = r.origin;
	HPoint point(p.x, p.y, p.z, 1);
	HVector vector(v.x, v.y, v.z, 0);

	return Ray(trans * point, trans * vector);
}

bool Entity::solve_polynomial_2(float a, float b, float c, float& t) const
{
    float delta = b * b - 4 * a * c;
    t = -1;

    // - pas de solution réelle
    if (delta < 0)
        return false;

    // 1 solution réelle
    if (delta == 0)
        t = -b / (2 * a);

    // - deux solutions réelles
    else
    {
        float t1 = (-b - sqrt(delta)) / (2 * a);
        float t2 = (-b + sqrt(delta)) / (2 * a);

        // - le rayon est dirigé à l'opposé du cylindre
        if (t1 < 0 && t2 < 0)
            return false;

        // - le rayon est dirigé vers le cylindre et a deux intersections
        if (t1 > 0 && t2 > 0)
            t = t1 < t2 ? t1 : t2;

        // - origine du rayon dans le cylindre et seul t1 visible
        else if (t1 > 0)
            t = t1;

        // - origine du rayon dans le cylindre et seul t2 visible
        else
            t = t2;
    }

    if (t < 0)
        return false;

    return true;
}

bool Entity::is_epsilon(float value, float test, float delta) const
{
    return (test > value - delta && test < value + delta);
}