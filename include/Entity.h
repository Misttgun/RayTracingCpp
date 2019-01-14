#pragma once
#ifndef ENTITY_H
#define ENTITY_H

#include "Matrix.h"
#include "Ray.h"
#include "Point.h"

#include <utility>

class Entity
{
public:
	Entity();
	Entity(float x, float y, float z);
	Entity(const Entity& entity);
	~Entity() = default;

	Entity& operator=(const Entity& other);

	friend void swap(Entity& first, Entity& second) noexcept
	{
		using std::swap;
		swap(first.position, second.position);
		swap(first.trans, second.trans);
		swap(first.transInv, second.transInv);
	}

	void translate(float x, float y, float z);
	void rotate_x(float deg);
	void rotate_y(float deg);
	void rotate_z(float deg);
	void scale(float factor);

    bool solve_polynomial_2(float a, float b, float c, float& t) const;
    bool is_epsilon(float value, float test, float delta) const;

	Point local_to_global(const Point& p) const;
	Vector local_to_global(const Vector& v) const;
	Ray local_to_global(const Ray& r) const;
	Point global_to_local(const Point& p) const;
	Vector global_to_local(const Vector& v) const;
	Ray global_to_local(const Ray& r) const;

protected:
	Point position;
	Matrix trans;
	Matrix transInv;
};
#endif