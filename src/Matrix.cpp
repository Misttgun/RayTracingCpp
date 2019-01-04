#include "Matrix.h"

#include <utility>

Matrix::Matrix()
	: m_rows(4), m_cols(4)
{
	m_tab = new float[16];
	for (int i = 0; i < 16; i++)
		m_tab[i] = 0.0f;
}

Matrix::Matrix(const Matrix & mat)
{
	m_rows = mat.getRows();
	m_cols = mat.getCols();
	m_tab = new float[m_rows * m_cols];
	for (int i = 0; i < m_rows; i++)
		for (int j = 0; j < m_cols; j++)
			set(i, j, mat.get(i, j));
}

Matrix::~Matrix()
{
	delete[] m_tab;
}

Matrix & Matrix::operator=(const Matrix& other)
{
	Matrix temp(other);
	swap(*this, temp);
	return *this;
}

float& Matrix::operator()(int i, int j)
{
	if ((i * m_cols + j) >= (m_cols * m_rows))
		return m_tab[0];

	return m_tab[i * m_cols + j];
}

float Matrix::operator()(int i, int j) const
{
	if ((i * m_cols + j) >= (m_cols * m_rows))
		return m_tab[0];

	return get(i, j);
}

void Matrix::swap(Matrix& first, Matrix& second)
{

	std::swap(first.m_rows, second.m_rows);
	std::swap(first.m_cols, second.m_cols);
	std::swap(first.m_tab, second.m_tab);
}

void Matrix::set(int i, int j, int val)
{
	m_tab[i * m_cols + j] = val;
}
