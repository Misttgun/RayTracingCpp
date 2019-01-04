#include "Vector.h"
#include "HVector.h"
#include <iostream>

int main() {
	HVector vec(1, 2, 3, 2);
	HVector vac(4, 5, 6, 3);

	HVector res = vac - vec;
	//float res = vac.dot(vec);

	std::cout << "(" << res.x << ", " << res.y << ", " << res.z << ", " << res.w << ")" << std::endl;
	//std::cout << "Res = " << res << std::endl;
	getchar();
	return 0;
}