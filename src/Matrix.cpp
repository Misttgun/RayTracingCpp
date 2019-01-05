#include "Matrix.h"
#include "HPoint.h"

#include <iostream>
#include <cassert>

Matrix::Matrix()
{
	for (auto& i : m_tab)
		for (float& j : i)
			j = 0.0f;
}

Matrix::Matrix(const Matrix & mat)
{
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			m_tab[i][j] = mat(i, j);
}

Matrix & Matrix::operator=(const Matrix& other)
{
	Matrix temp(other);
	swap(*this, temp);
	return *this;
}

float& Matrix::operator()(int i, int j)
{
	if (i >= 4 || j >= 4)
		return m_tab[0][0];

	return m_tab[i][j];
}

float Matrix::operator()(int i, int j) const
{
	if (i >= 4 || j >= 4)
		return m_tab[0][0];

	return get(i, j);
}

Matrix Matrix::operator*(const Matrix& mat) const
{
	Matrix product;

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			for (int k = 0; k < 4; k++)
			{
				product(i, j) += m_tab[i][k] * mat(k, j);
			}
		}
	}

	return product;
}

HVector Matrix::operator*(const HVector& vec) const
{
	HVector product;

	for (int i = 0; i < 4; i++)
	{
		for (int k = 0; k < 4; k++)
		{
			product[i] += m_tab[i][k] * vec[k];
		}
	}

	return product;
}

void Matrix::set(int i, int j, float val)
{
	m_tab[i][j] = val;
}

void Matrix::get_cofactor(const Matrix& mat, Matrix& temp, int p, int q, int n)
{
	int i = 0, j = 0;

	for (int row = 0; row < n; ++row)
	{
		for (int col = 0; col < n; ++col)
		{
			if (row != p && col != q)
			{
				temp(i, j++) = mat(row, col);

				if (j == n - 1)
				{
					j = 0;
					i++;
				}
			}
		}
	}
}

float Matrix::determinant(const Matrix& mat, int n)
{
	float res = 0;

	if (n == 1)
		return mat(0, 0);

	Matrix temp; //cofactors
	int sign = 1; //sign multiplier

	for (int f = 0; f < n; ++f)
	{
		get_cofactor(mat, temp, 0, f, n);
		res += sign * mat(0, f) * determinant(temp, n - 1);

		sign = -sign;
	}

	return res;
}

void Matrix::adjoint(Matrix& adj) const
{
	Matrix temp;
	int sign;

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			get_cofactor(*this, temp, i, j, 4);
			sign = ((i + j) % 2 == 0) ? 1 : -1;
			adj(j, i) = sign * determinant(temp, 3);
		}
	}
}

Matrix Matrix::inverse() const
{
	float det = determinant(*this, 4);
	assert(det != 0);

	Matrix adj;
	adjoint(adj);

	Matrix inverse;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			inverse(i, j) = adj(i, j) / det;
		}
	}

	return inverse;
}

void Matrix::display()
{
	for (auto& i : m_tab)
	{
		for (float j : i)
		{
			std::cout << j << " ";
		}
		std::cout << std::endl;
	}
}
