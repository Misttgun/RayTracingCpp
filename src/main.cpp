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
#include "Cone.h"

int main()
{
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

    Material mat_red(Color(1, 0, 0), Color(1, 0, 0), Color(1, 0, 0), 10);
    Material mat_blue(Color(0, 0, 1), Color(0, 0, 1), Color(0, 0, 1), 5);
    Material mat_green(Color(0, 1, 0), Color(0, 1, 0), Color(0, 1, 0), 1);
    Material mat_grey(Color(0.5, 0.5, 0.5), Color(0.5, 0.5, 0.5), Color(0.5, 0.5, 0.5), 1);

    //std::shared_ptr<Sphere> p = std::make_shared<Sphere>(Sphere());
    std::shared_ptr<Plan> p = std::make_shared<Plan>(Plan());
    p->rotate_x(-180);
    p->translate(-3, 6, -6);
    //p->scale(10.0f);
    p->set_material(mat_grey);
    scene.add_object(p);

    // FRONT IS BLUE
    //std::shared_ptr<Sphere> p2 = std::make_shared<Sphere>(Sphere());
    ////std::shared_ptr<Square> p2 = std::make_shared<Square>(Square());
    //p2->translate(0, 1, 0);
    //p2->set_material(mat_blue);
    //scene.add_object(p2);

 //   // RIGHT IS GREEN
    //std::shared_ptr<Sphere> p3 = std::make_shared<Sphere>(Sphere());
    //p3->translate(1, 0, 2);
    //p3->set_material(mat_green);
    //scene.add_object(p3);

    // LEFT IS RED
    std::shared_ptr<Sphere> p4 = std::make_shared<Sphere>(Sphere());
    p4->rotate_y(45);
    p4->translate(2.5, 0, -2);
    p4->scale(1.0f);
    p4->set_material(mat_blue);
    scene.add_object(p4);

    std::shared_ptr<Cylinder> p5 = std::make_shared<Cylinder>(Cylinder());
    //p5->rotate_x(45);
    p5->translate(1, 0, -4);
    p5->scale(1.0f);
    p5->set_material(mat_green);
    scene.add_object(p5);

    //   std::shared_ptr<Cone> p6 = std::make_shared<Cone>(Cone());
    //   p6->translate(0, 0, 0);
    //   p6->scale(2);
    //   //p5->rotate_x(45);
    //   p6->set_material(mat_red);
    //   scene.add_object(p6);

    /*
    std::shared_ptr<Cube> p7 = std::make_shared<Cube>(Cube());
    p7->rotate_y(45);
    p7->rotate_x(45);
    p7->translate(-2, 0, -2);
    p7->scale(1.f);
    p7->set_material(mat_red);
    scene.add_object(p7);
    */

    std::shared_ptr<Cone> cone = std::make_shared<Cone>(Cone());
    cone->translate(-2, 0, -2);
    //cone->rotate_x(90);
    cone->set_material(mat_red);
    cone->scale(1.0f),
    scene.add_object(cone);


    std::shared_ptr<Light> l = std::make_shared<Light>(Light(2, -1.5, 0));
    l->id = Color(0.4f, 0.4f, 0.4f);
    l->is = Color(0.8f, 0.8f, 0.8f);
    scene.add_light(l);

    // SETTING CAMERA
    Camera test;
    test.focal = 2;
    test.translate(0, -2, 5);
    test.rotate_x(-30);
    scene.set_camera(test);


    // SET META DATA
    scene.set_bg(Color(0, 0, 0));
    scene.set_ambiant(Color(0.5, 0.5, 0.5));

    std::cout << "Scene is loaded with " << scene.nb_objects() << " objects and "
        << scene.nb_lights() << " lights\n";


    Renderer renderer;
    Camera cam = scene.get_camera();

    const int size = 800;

    Color** render_2 = new Color*[size];
    for (int i = 0; i < size; i++)
        render_2[i] = new Color[size];

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

            else
            {
                //render_2[i][j] = intersected->get_material().ka;
                render_2[i][j] = renderer.get_impact_color(ray, *intersected, impact, scene);
                max = std::max(std::max(std::max(render_2[i][j].r, render_2[i][j].g), render_2[i][j].b), max);
            }
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
