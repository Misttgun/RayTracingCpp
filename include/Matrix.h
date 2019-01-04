#pragma once
#ifndef MATRIX_H
#define MATRIX_H

class Matrix
{
public:
	Matrix();
	Matrix(const Matrix& mat);
	~Matrix();

	Matrix& operator=(const Matrix& other);
	float& operator()(int i, int j);
	float operator()(int i, int j) const;

	void swap(Matrix& first, Matrix& second);
	int getCols() const { return m_cols; }
	int getRows() const { return m_rows; }
	void set(int i, int j, int val);
	int get(int i, int j) const { return m_tab[i * m_cols + j]; }

	//TODO  Ajouter l'operateur * et la méthode inverse

private:
	int m_rows;
	int m_cols;
	float* m_tab;

};
#endif