#include "Entity.h"
#include "HVector.h"

#include <cmath>
#include <iostream>


Entity::Entity()
{
    const Matrix mat(1.0f);

    trans = mat;
    transInv = mat;
}

Entity::Entity(const float x, const float y, const float z)
{
    const Matrix mat(1.0f);
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

    Matrix mat(1.0f);
    mat(1, 1) = cos(deg);
    mat(1, 2) = -sin(deg);
    mat(2, 1) = sin(deg);
    mat(2, 2) = cos(deg);

    trans = mat * trans;
    transInv = trans.inverse();
}

void Entity::rotate_y(const float deg)
{
    std::cout << "Rotate y " << deg << std::endl;
    Matrix mat(1.0f);

    mat(0, 0) = cos(deg);
    mat(0, 2) = sin(deg);
    mat(2, 0) = -sin(deg);
    mat(2, 2) = cos(deg);

    trans = mat * trans;
    transInv = trans.inverse();
}

void Entity::rotate_z(const float deg)
{
    std::cout << "Rotate z " << deg << std::endl;

    Matrix mat(1.0f);
    mat(0, 0) = cos(deg);
    mat(0, 1) = -sin(deg);
    mat(1, 0) = sin(deg);
    mat(1, 1) = cos(deg);

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


// - point coordinates
Vector Entity::local_to_global_point(const Vector& vec) const
{
    const HVector h_ori(vec.x, vec.y, vec.z, 1.f);
    HVector res = trans * h_ori;

    return Vector(res.x, res.y, res.z);
}

Vector Entity::global_to_local_point(const Vector& vec) const
{
    const HVector h_ori(vec.x, vec.y, vec.z, 1.f);
    HVector res = transInv * h_ori;

    return Vector(res.x, res.y, res.z);
}

// - vector coordinates
Vector Entity::local_to_global_vector(const Vector& vec) const
{
    const HVector h_vec(vec.x, vec.y, vec.z, 0.f);
    HVector res = trans * h_vec;

    return Vector(res.x, res.y, res.z);
}

Vector Entity::global_to_local_vector(const Vector& vec) const
{
    const HVector h_vec(vec.x, vec.y, vec.z, 0.f);
    HVector res = transInv * h_vec;

    return Vector(res.x, res.y, res.z);
}

// - ray coordinates
Ray Entity::local_to_global(const Ray & r) const
{
    const Vector dir = r.direction;
    const Vector ori = r.origin;

    const HVector h_ori(ori.x, ori.y, ori.z, 1.f);
    const HVector h_dir(dir.x, dir.y, dir.z, 0.f);

    return Ray(trans * h_ori, trans * h_dir);
}

Ray Entity::global_to_local(const Ray& r) const
{
    const Vector dir = r.direction;
    const Vector ori = r.origin;

    const HVector h_ori(ori.x, ori.y, ori.z, 1.f);
    const HVector h_dir(dir.x, dir.y, dir.z, 0.f);

    return Ray(transInv * h_ori, transInv * h_dir);
}

bool Entity::solve_polynomial_2(float a, float b, float c, float& t) const
{
    const float delta = b * b - 4 * a * c;
    t = -1.0f;

    // - pas de solution réelle
    if (delta < 0.0f)
        return false;

    // 1 solution réelle
    if (delta <= 0.0001f  && delta >= -0.0001f)
    {
        t = -b / (2.0f * a);
    }

    // - deux solutions réelles
    else
    {
        float t1 = (-b - sqrt(delta)) / (2.0f * a);
        float t2 = (-b + sqrt(delta)) / (2.0f * a);

        // - le rayon est dirigé à l'opposé de l'objet
        if (t1 < 0.0f && t2 < 0.0f)
            return false;

        // - le rayon est dirigé vers l'objet et a deux intersection
        if (t1 > 0.0f && t2 > 0.0f)
            t = t1 < t2 ? t1 : t2;

        // - origine du rayon dans l'objet et seul t1 visibl
        else if (t1 > 0.0f)
            t = t1;

        // - origine du rayon dans l'objet t seul t2 visible
        else if (t2 > 0.0f)
            t = t2;
    }


    return (t > 0.0f);
}

bool Entity::solve_full_polynomial_2(float a, float b, float c, float& t1, float& t2) const
{
    const float delta = b * b - 4 * a * c;

    // - pas de solution réelle
    if (delta < 0.0f)
        return false;

    // 1 solution réelle
    if (delta <= 0.0001f  && delta >= -0.0001f)
    {
        t1 = -b / (2.0f * a);
	t2 = t1;
    }

    // - deux solutions réelles
    else
    {
        t1 = (-b - sqrt(delta)) / (2.0f * a);
        t2 = (-b + sqrt(delta)) / (2.0f * a);
    }

    return true;
}


// - use only by solve_polynomial_4. Only return one real solution, DOES NOT
// RESOLVE ALL THE SYSTEM
void Entity::solve_polynomial_3(float a, float b, float c, float &t) const
{
    // polynom must be of the form x^3 + ax^2 + bx + c

    float p = (-1.0f/3.0f)*pow(a, 2.0f) + b;
    float q = (2.0f/27.0f)*pow(a, 3.0f) + (-1.0f/3.0f)*a*b + c;

    // let y = x - a/3, polynom is now y^3 + py + q

    // simplify arithmetics
    float p_ = p / 3.0f;
    float q_ = q / 2.0f;
    float delta_ = -(pow(p_, 3.0f) + pow(q_, 2.0f));

    float y;

    // 1 real solution
    if (delta_ < 0.0f)
    {
	float r = cbrt(-q_ + sqrt(-delta_));
	float s = cbrt(-q_ - sqrt(-delta_));
	y = r + s;
    }

    // 2 real solutions
    else if (delta_ > -0.00001f && delta_ < 0.00001f)
    {
	float r = cbrt(-q_);
	y = 2.0f*r;
    }

    // 3 solutions
    else
    {
	float theta = (1.0f/3.0f)*acos(-q_ / sqrt(-pow(p_, 3.0f)));
	y = 2.0f*sqrt(-p_)*cos(theta);
    }

    // x = y + 3/a;
    t = y - a/3.0f;
}

bool Entity::solve_polynomial_4(float a, float b, float c, float d, float &t) const
{
    // polynom must be of the form x^4 + ax^3 + bx^2 +cx + d;
    
    float p = (-3.0f/8.0f)*pow(a, 2.0f) + b;
    float q = (1.0f/8.0f)*pow(a, 3.0f) - (1.0f/2.0f)*a*b + c;
    float r = (-3.0f/256.0f)*pow(a, 4.0f) + (1.0f/16.0f)*pow(a, 2.0f)*b + (-1.0f/4.0f)*a*c + d;
    float t1 = -INFINITY, t2 = -INFINITY, t3 = -INFINITY, t4 = -INFINITY;

    // give us a 3rd degree polynomial : y^3 - (p/2)y^2 - ry + (4rp-q^2) / 8 = 0
    float y;
    solve_polynomial_3(-p/2.0f, -r, (4.0f*r*p - q*q)/8.0f, y);

    // after we get one real solution (there's always one)
    float b_q = sqrt(2.0f*y - p);
    float c_q = sqrt(y*y - r);

 
    if (q >= 0.0f)
    {
	// solutions of the quartic are the solutions of 
	// x^2 + x*sqrt(2y - p) + y - sqrt(y^2 - r) = 0
	// x^2 - x*sqrt(2y - p) + y + sqrt(y^2 - r) = 0

	solve_full_polynomial_2(1.0f, b_q, y-c_q, t1, t2);
       	solve_full_polynomial_2(1.0f, -b_q, y+c_q, t3, t4);
	 
    }

    else
    {
	// solutions of the quartic are the solutions of 
	// x^2 + x*sqrt(2y - p) + y + sqrt(y^2 - r) = 0
	// x^2 - x*sqrt(2y - p) + y - sqrt(y^2 - r) = 0
	solve_full_polynomial_2(1.0f, b_q, y+c_q, t1, t2);
       	solve_full_polynomial_2(1.0f, -b_q, y-c_q, t3, t4);
    }
    t1 -= a/4.0f;
    t2 -= a/4.0f;
    t3 -= a/4.0f;
    t4 -= a/4.0f;

    if (t1 < 0.0f && t2 < 0.0f && t3 < 0.0f && t4 < 0.0f)
	return false;

    t = INFINITY;

    if (t1 >= 0.0f && t1 < t)
	t = t1;

    if (t2 >= 0.0f && t2 < t)
	t = t2;
    
    if (t3 >= 0.0f && t3 < t)
	t = t3;
    
    if (t4 >= 0.0f && t4 < t)
	t = t4;

    if (t == INFINITY)
	return false;
    return t > 0;
}

bool Entity::is_epsilon(float value, float test, float delta) const
{
    return (test > value - delta && test < value + delta);
}
