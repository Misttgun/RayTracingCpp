#include "Scene.h"
#include "Vector.h"
#include "HVector.h"
#include "Matrix.h"
#include "Renderer.h"

#include <iostream>
#include <memory>
#include <fstream>
#include "Plan.h"
#include "Sphere.h"
#include "Square.h"
#include "Cylinder.h"
#include <algorithm>
#include "Cube.h"

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

	Material mat(Color(1, 0, 0), Color(1, 0, 0), Color(1, 0, 0), 1);
	Material mat2(Color(0, 0, 1), Color(0, 0, 1), Color(0, 0, 1), 1);
    Material mat3(Color(0.2, 0.5, 0.7), Color(0.8, 0.8, 0.8), Color(0.2, 0.2, 0.2), 0.9);

	//std::shared_ptr<Sphere> p = std::make_shared<Sphere>(Sphere());
	std::shared_ptr<Plan> p = std::make_shared<Plan>(Plan());
	p->translate(0, 0, 6);
	p->set_material(mat);
	scene.add_object(p);

	std::shared_ptr<Sphere> p2 = std::make_shared<Sphere>(Sphere());
	//std::shared_ptr<Square> p2 = std::make_shared<Square>(Square());
	p2->translate(0, 0, 1);
	p2->set_material(mat2);
	scene.add_object(p2);

	std::shared_ptr<Sphere> p3 = std::make_shared<Sphere>(Sphere());
	p3->translate(1, 1, 4);
	p3->set_material(mat3);
	scene.add_object(p3);

	std::shared_ptr<Sphere> p4 = std::make_shared<Sphere>(Sphere());
	p4->translate(-1, 1, 4);
	p4->set_material(mat2);
	scene.add_object(p4);

    std::shared_ptr<Cube> p5 = std::make_shared<Cube>(Cube());
    p5->translate(-1, 1, 3);
    //p5->rotate_x(45);
    p5->set_material(mat3);
    scene.add_object(p5);


	std::shared_ptr<Light> l = std::make_shared<Light>(Light(2, 2, 0));
	l->id = Color(0.0f, 0.0f, 0.5f);
	l->is = Color(0.0f, 0.0f, 0.7f);
	scene.add_light(l);

	scene.set_camera(Camera(0, 0, 0, 3));
	scene.set_bg(Color(1, 1, 1));
	scene.set_ambiant(Color(0.5, 0.5, 0.5));

	std::cout << "Scene is loaded with " << scene.nb_objects() << " objects and "
		<< scene.nb_lights() << " lights\n";


	Renderer renderer;
	Camera cam = scene.get_camera();
	//const int width = 200, height = 200;
	//Color render[width][height];


    const int size = 800;

    Color** render_2 = new Color*[size];
    for (int i = 0; i < size; i++)
        render_2[i] = new Color[size];

	bool has_impact;
    float max = -INFINITY;

	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			float x = static_cast<float>(j) / size, y = static_cast<float>(i) / size;

			Ray ray = cam.get_ray(x, y);
            Vector impact;
			std::shared_ptr<Object> intersected = scene.closer_intersected(ray, impact);

			if (intersected == nullptr)
			{
				render_2[i][j] = scene.get_background();
				continue;
			}

			render_2[i][j] = renderer.get_impact_color(ray, *intersected, impact, scene);
            max = std::max(std::max(std::max(render_2[i][j].r, render_2[i][j].g), render_2[i][j].b), max);
		}
	}

	std::ofstream file_ppm;
	file_ppm.open("render.ppm");

	//file_ppm << "P3\n" << width << " " << height << "\n255\n";
    file_ppm << "P3\n" << size << " " << size << "\n255\n";

	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
            if (render_2[i][j].r > 1.f || render_2[i][j].g > 1.f || render_2[i][j].b > 1.f)
            {
                render_2[i][j].r /= max;
                render_2[i][j].g /= max;
                render_2[i][j].b /= max;
            }

			file_ppm << render_2[i][j].r * 255 << " " << render_2[i][j].g * 255 << " " << render_2[i][j].b * 255 << " ";
		}

		file_ppm << "\n";
	}
	//std::cout << "Res = " << res << std::endl;
	std::cout << "DONE !\n";

    for (auto i = 0; i < size; i++)
        delete render_2[i];

    delete[] render_2;

	getchar();
	return 0;
}
