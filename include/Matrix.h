#pragma once
#ifndef MATRIX_H
#define MATRIX_H

#include <utility>

class HVector;

class Matrix
{
public:
	Matrix();
	Matrix(const Matrix& mat);
	~Matrix()=default;

	Matrix& operator=(const Matrix& other);
	float& operator()(int i, int j);
	float operator()(int i, int j) const;
	Matrix operator*(const Matrix& mat) const;
	HVector operator*(const HVector& vec) const;

	friend void swap(Matrix& first, Matrix& second) noexcept
	{
		std::swap(first.m_tab, second.m_tab);
	}

	void set(int i, int j, float val);
	float get(int i, int j) const { return m_tab[i][j]; }

	static void get_cofactor(const Matrix& mat, Matrix& temp, int p, int q, int n);
	static float determinant(const Matrix& mat, int n);
	void adjoint(Matrix& adj) const;
	Matrix inverse() const;

	void display();

	//TODO  Ajouter l'operateur * et la méthode inverse

private:
	float m_tab[4][4]{};

};
#endif