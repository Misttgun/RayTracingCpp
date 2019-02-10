#pragma once
#ifndef ENTITY_H
#define ENTITY_H

#include "Matrix.h"
#include "Ray.h"

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
    void solve_polynomial_3(float a, float b, float c, float& t) const;
    bool solve_polynomial_4(float a, float b, float c, float d, float &t) const;
    bool is_epsilon(float value, float test, float delta) const;

    // - ray coordinates
    Ray local_to_global(const Ray& r) const;
	Ray global_to_local(const Ray& r) const;

    // - point coordinates
    Vector local_to_global_point(const Vector& vec) const;
    Vector global_to_local_point(const Vector& vec) const;

    // - vector coordinates
    Vector local_to_global_vector(const Vector& vec) const;
    Vector global_to_local_vector(const Vector& vec) const;

protected:
	Vector position;
	Matrix trans;
	Matrix transInv;
};
#endif
