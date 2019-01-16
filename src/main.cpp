#include "Scene.h"
#include "Vector.h"
#include "Point.h"
#include "HVector.h"
#include "HPoint.h"
#include "Matrix.h"
#include "Renderer.h"

#include <iostream>
#include <memory>
#include <fstream>
#include "Plan.h"
#include "Sphere.h"
#include "Square.h"

int main() {
	/*
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
	*/
	Scene scene;

	//scene.load("config.txt");

	Material mat(Color(1, 0, 0), Color(0.5, 0, 0), Color(0.2, 0, 0), 1);
	Material mat2(Color(0, 0, 1), Color(0.5f, 0.5f, 0.5f), Color(0.2f, 0.2f, 0.2f), 0.9f);

	//std::shared_ptr<Sphere> p = std::make_shared<Sphere>(Sphere());
	//std::shared_ptr<Plan> p = std::make_shared<Plan>(Plan());
	//p->translate(0, 0, 2);
	////p->rotate_y(-45);
	//p->set_material(mat);
	//scene.add_object(p);

	std::shared_ptr<Sphere> p2 = std::make_shared<Sphere>(Sphere());
	//std::shared_ptr<Square> p2 = std::make_shared<Square>(Square());
	p2->translate(0, 0, 1);
	p2->set_material(mat2);
	scene.add_object(p2);

	std::shared_ptr<Sphere> p3 = std::make_shared<Sphere>(Sphere());
	p3->translate(1, 1, 4);
	p3->set_material(mat2);
	scene.add_object(p3);

	std::shared_ptr<Sphere> p4 = std::make_shared<Sphere>(Sphere());
	p4->translate(-1, 1, 4);
	p4->set_material(mat2);
	scene.add_object(p4);


	std::shared_ptr<Light> l = std::make_shared<Light>(Light(0, 2, 0));
	l->id = Color(0.2f, 0.2f, 0.2f);
	l->is = Color(0.9f, 0.9f, 0.9f);
	scene.add_light(l);

	scene.set_camera(Camera(0, 0, 0, 3));
	scene.set_bg(Color(0, 0, 0));
	scene.set_ambiant(Color(0.6f, 0.6f, 0.6f));

	std::cout << "Scene is loaded with " << scene.nb_objects() << " objects and "
		<< scene.nb_lights() << " lights\n";


	Renderer renderer;
	Camera cam = scene.get_camera();
	const int width = 200, height = 200;
	Color render[width][height];
	bool has_impact;

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			float x = static_cast<float>(j) / width, y = static_cast<float>(i) / height;

			Ray ray = cam.get_ray(x, y);
			Point impact;
			std::shared_ptr<Object> intersected = scene.closer_intersected(ray, impact);

			if (intersected == nullptr)
			{
				render[i][j] = scene.get_background();
				continue;
			}

			render[i][j] = renderer.get_impact_color(ray, *intersected, impact, scene);

		}
	}

	std::ofstream file_ppm;
	file_ppm.open("render.ppm");

	file_ppm << "P3\n" << width << " " << height << "\n255\n";

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			file_ppm << render[i][j].r * 255 << " " << render[i][j].g * 255 << " " << render[i][j].b * 255 << " ";
		}

		file_ppm << "\n";
	}
	//std::cout << "Res = " << res << std::endl;
	std::cout << "DONE !\n";
	getchar();
	return 0;
}
