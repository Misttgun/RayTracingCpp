#include "Entity.h"
#include "HVector.h"

#include <cmath>
#include <iostream>
#define M_PI static_cast<float>(3.14159265)


Entity::Entity ()
{
	const Matrix mat(1.0f);

    trans = mat;
    transInv = mat;
}

Entity::Entity(const float x, const float y, const float z)
{
	const Matrix mat(1.f);
    trans = mat;
    transInv = mat;

    translate(x, y, z);

}

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
    std::cout << "Translate " << x << "," << y << "," << z << std::endl;

	Matrix mat(1.0f);
	mat(0, 3) = x;
	mat(1, 3) = y;
	mat(2, 3) = z;

	trans = mat * trans;

	transInv = trans.inverse();
}

void Entity::rotate_x(const float deg)
{
    std::cout << "Rotate x " << deg << std::endl;
	const float rad = deg * M_PI / 180.f;

	Matrix mat(1.0f);
	mat(1, 1) = cos(rad);
	mat(1, 2) = -sin(rad);
	mat(2, 1) = sin(rad);
	mat(2, 2) = cos(rad);
	
    trans = mat * trans;

	transInv = trans.inverse();
}

void Entity::rotate_y(const float deg)
{
    std::cout << "Rotate y " << deg << std::endl;
	const float rad = deg * M_PI / 180.f;
	Matrix mat(1.0f);

	mat(0, 0) = cos(rad);
	mat(0, 2) = sin(rad);
	mat(2, 0) = -sin(rad);
	mat(2, 2) = cos(rad);

	trans = mat * trans;

	transInv = trans.inverse();
}

void Entity::rotate_z(const float deg)
{
    std::cout << "Rotate z " << deg << std::endl;
	const float rad = deg * M_PI / 180.f;

	Matrix mat(1.0f);
	mat(0, 0) = cos(rad);
	mat(0, 1) = -sin(rad);
	mat(1, 0) = sin(rad);
	mat(1, 1) = cos(rad);

	trans = mat * trans;
	transInv = trans.inverse();
}

void Entity::scale(const float factor)
{
    std::cout << "Scale " << factor << std::endl;

	Matrix mat(1.0f);
	mat(0, 0) = factor;
	mat(1, 1) = factor;
	mat(2, 2) = factor;

    trans.display();

    std::cout << "--- scale factor << " << factor << std::endl;

    trans = mat * trans;

    trans.display();

	transInv = trans.inverse();


    std::cout << " --- \n";
    transInv.display();
}

/*
Point Entity::local_to_global(const Point & p) const
{
	const HPoint point(p.x, p.y, p.z, 1);
	return  transInv * point;
}

Vector Entity::local_to_global(const Vector & v) const
{
	const HVector vector(v.x, v.y, v.z, 0);
	return  transInv * vector;
}


Point Entity::global_to_local(const Point & p) const
{
	const HPoint point(p.x, p.y, p.z, 1);
	return  trans * point;
}

Vector Entity::global_to_local(const Vector & v) const
{
	const HVector vector(v.x, v.y, v.z, 0);
	return  trans * vector;
}*/

// - point coordinates
Vector Entity::local_to_global_point(const Vector& vec) const
{
    const HVector h_ori(vec.x, vec.y, vec.z, 1.f);
    HVector res = transInv * h_ori;

    return Vector(res.x, res.y, res.z);
}

Vector Entity::global_to_local_point(const Vector& vec) const
{
    const HVector h_ori(vec.x, vec.y, vec.z, 1.f);
    HVector res = trans * h_ori;

    return Vector(res.x, res.y, res.z);
}

// - vector coordinates
Vector Entity::local_to_global_vector(const Vector& vec) const
{
    const HVector h_vec(vec.x, vec.y, vec.z, 0.f);
    HVector res = transInv * h_vec;

    return Vector(res.x, res.y, res.z);
}

Vector Entity::global_to_local_vector(const Vector& vec) const
{
    const HVector h_vec(vec.x, vec.y, vec.z, 0.f);
    HVector res = trans * h_vec;

    return Vector(res.x, res.y, res.z);
}

// - ray coordinates
Ray Entity::local_to_global(const Ray & r) const
{
    const Vector dir = r.direction;
    const Vector ori = r.origin;

    const HVector h_ori(ori.x, ori.y, ori.z, 1.f);
    const HVector h_dir(dir.x, dir.y, dir.z, 0.f);

    return Ray(transInv * h_ori, transInv * h_dir);
}

Ray Entity::global_to_local(const Ray& r) const
{
	const Vector dir = r.direction;
	const Vector ori = r.origin;

	const HVector h_ori(ori.x, ori.y, ori.z, 1.f);
	const HVector h_dir(dir.x, dir.y, dir.z, 0.f);

	return Ray(trans * h_ori, trans * h_dir);
}

bool Entity::solve_polynomial_2(float a, float b, float c, float& t) const
{
	const float delta = b * b - 4 * a * c;
    t = -1.0f;

    // - pas de solution réelle
    if (delta < 0)
        return false;

    // 1 solution réelle
    if (delta <= 0.0001f  && delta >= -0.0001f)
        t = -b / (2 * a);

    // - deux solutions réelles
    else
    {
        float t1 = (-b - sqrt(delta)) / (2 * a);
        float t2 = (-b + sqrt(delta)) / (2 * a);

        // - le rayon est dirigé à l'opposé de l'objet
        if (t1 < 0.f && t2 < 0.f)
            return false;

        // - le rayon est dirigé vers l'objet et a deux intersections
        if (t1 > 0.f && t2 > 0.f)
            t = t1 < t2 ? t1 : t2;

        // - origine du rayon dans l'objet et seul t1 visible
        else if (t1 > 0.f)
            t = t1;

        // - origine du rayon dans l'objet t seul t2 visible
        else
            t = t2;
    }

    if (t < 0.f)
        return false;

    return true;
}

bool Entity::is_epsilon(float value, float test, float delta) const
{
    return (test > value - delta && test < value + delta);
}