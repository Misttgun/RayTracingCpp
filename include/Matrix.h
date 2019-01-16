#pragma once
#ifndef MATRIX_H
#define MATRIX_H

#include <utility>

class HVector;

class Matrix
{
public:
	Matrix();
    Matrix(float diag);
	Matrix(const Matrix& mat);
	~Matrix()=default;

	Matrix& operator=(const Matrix& other);
	float& operator()(int i, int j);
	float operator()(int i, int j) const;
	Matrix operator*(const Matrix& mat) const;
	HVector operator*(const HVector& vec) const;

	friend void swap(Matrix& first, Matrix& second) noexcept
	{
		using std::swap;
		swap(first.m_tab, second.m_tab);
	}

	void set(int i, int j, float val);
	float get(const int i, const int j) const { return m_tab[i][j]; }

	static void get_cofactor(const Matrix& mat, Matrix& temp, int p, int q, int n);
	static float determinant(const Matrix& mat, int n);
	void adjoint(Matrix& adj) const;
	Matrix inverse() const;

	void display();

private:
	float m_tab[4][4]{};

};
#endif