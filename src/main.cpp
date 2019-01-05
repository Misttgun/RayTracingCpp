#include "Vector.h"
#include "Point.h"
#include "HVector.h"
#include "HPoint.h"
#include "Matrix.h"

#include <iostream>

int main() {
	HPoint vec(1, 2, 3, 2);
	HPoint vac(4, 5, 6, 3);

	HVector res = vac - vec;
	//float res = vac.dot(vec);

	Point vec1(1, 2, 3);
	Point vac1(4, 5, 6);

	Vector res1 = vac1;

	Matrix mat;
	mat(0, 0) = 5;
	mat(0, 1) = -2;
	mat(0, 2) = 2;
	mat(0, 3) = 7;

	mat(1, 0) = 1;
	mat(1, 1) = 0;
	mat(1, 2) = 0;
	mat(1, 3) = 3;

	mat(2, 0) = -3;
	mat(2, 1) = 1;
	mat(2, 2) = 5;
	mat(2, 3) = 0;

	mat(3, 0) = 3;
	mat(3, 1) = -1;
	mat(3, 2) = -9;
	mat(3, 3) = 4;

	Matrix adj;
	mat.adjoint(adj);

	Matrix inverse = mat.inverse();

	HVector product = mat * vec;

	std::cout << "(" << product[0] << ", " << product[1] << ", " << product[2] << ", " << product[3] << ")" << std::endl;
	std::cout << "-----------------------------------" << std::endl;
	std::cout << "(" << res.x << ", " << res.y << ", " << res.z << ", " << res.w << ")" << std::endl;
	std::cout << "-----------------------------------" << std::endl;
	std::cout << "(" << res1.x << ", " << res1.y << ", " << res1.z << ")" << std::endl;
	std::cout << "-----------------------------------" << std::endl;
	mat.display();
	std::cout << "-----------------------------------" << std::endl;
	adj.display();
	std::cout << "-----------------------------------" << std::endl;
	inverse.display();
	//std::cout << "Res = " << res << std::endl;
	getchar();
	return 0;
}