#include "Entity.h"
#include "HPoint.h"
#include "HVector.h"

#include <cmath>

Entity::Entity(float x, float y, float z)
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

void Entity::translate(float x, float y, float z)
{
	Matrix mat;
	mat(0, 3) = x;
	mat(1, 3) = y;
	mat(2, 3) = z;

	trans = mat * trans;
	transInv = trans.inverse();
}

void Entity::rotate_x(float deg)
{
	Matrix mat;
	mat(1, 1) = cos(deg);
	mat(1, 2) = -sin(deg);
	mat(2, 1) = sin(deg);
	mat(2, 2) = cos(deg);

	trans = mat * trans;
	transInv = trans.inverse();
}

void Entity::rotate_y(float deg)
{
	Matrix mat;
	mat(0, 0) = cos(deg);
	mat(0, 2) = sin(deg);
	mat(2, 0) = -sin(deg);
	mat(2, 2) = cos(deg);

	trans = mat * trans;
	transInv = trans.inverse();
}

void Entity::rotate_z(float deg)
{
	Matrix mat;
	mat(0, 0) = cos(deg);
	mat(0, 1) = -sin(deg);
	mat(1, 0) = sin(deg);
	mat(1, 1) = cos(deg);

	trans = mat * trans;
	transInv = trans.inverse();
}

void Entity::scale(float factor)
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
