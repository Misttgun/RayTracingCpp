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
#include "FCylinder.h"
#include <algorithm>
#include "Cube.h"
#include "Cone.h"
#include "FCone.h"
#include "Tore.h"
#include "Circle.h"

int main()
{
    const int scene_size = 800;
    Scene scene(scene_size);
    const int size = scene.image_size * scene._sampling_factor;
    //scene.load("config.txt");

    Material chalk_red(Color(1, 0, 0), 0.4f, 0.2f, 2, 0.25f, Type::Phong);
    Material chalk_green(Color(0, 1, 0), 0.4f, 0.2f, 2, 0.25f, Type::Phong);
    Material chalk_blue(Color(0, 0, 1), 0.4f, 0.2f, 2, 0.25f, Type::Phong);
    Material mirror_blue(Color(0, 0, 1), 1.0f, 0.5f, 1, 0.01f, Type::Reflection);
    Material metal_green(Color(0, 1, 0), 0.4f, 0.4f, 50, 0.01f, Type::Phong);
    Material chalk_grey(Color(0.5, 0.5, 0.5), 0.4f, 0.2f, 2, 0.25f, Type::Phong);
    Material glass_yellow(Color(1, 1, 0), 0.4f, 0.4f, 0.0001f, 0.1f, Type::Refraction);
    Material chalk_black(Color(0, 0, 0), 0.4f, 0.2f, 2, 0.25f, Type::Phong);

    std::shared_ptr<Plan> back = std::make_shared<Plan>(Plan());
    back->translate(0, 0, -8);
    /*back->rotate_x(90);
    back->translate(0, 5, 0);*/
    back->set_materials(chalk_red, chalk_black);
    //back->set_material(chalk_red);
    scene.add_object(back);

    /*std::shared_ptr<Plan> front = std::make_shared<Plan>(Plan());
    front->rotate_y(-180);
    front->translate(0, 0, 8);
    front->scale(10);
    front->set_material(chalk_grey);
    scene.add_object(front);*/

    // FRONT IS BLUE
    std::shared_ptr<Cube> p2 = std::make_shared<Cube>(Cube());
    //std::shared_ptr<Square> p2 = std::make_shared<Square>(Square());
    p2->translate(2.f, 0, -4);
    p2->scale(0.5f);
    p2->set_materials(chalk_grey, chalk_green);
    scene.add_object(p2);

    // RIGHT IS GREEN
     /*std::shared_ptr<Sphere> p3 = std::make_shared<Sphere>(Sphere());
     p3->translate(1, -1, -1);
     p3->set_material(metal_green);
     scene.add_object(p3);*/

     // LEFT IS RED
    std::shared_ptr<Sphere> p4 = std::make_shared<Sphere>(Sphere());
    //std::shared_ptr<Cube> p4 = std::make_shared<Cube>(Cube());
    //p4->rotate_x(45);
    //p4->rotate_y(45);
    p4->translate(1.75f, 0, 0);
    p4->scale(1.f);
    p4->set_material(glass_yellow);
    scene.add_object(p4);

    std::shared_ptr<Cylinder> p5 = std::make_shared<Cylinder>(Cylinder());
    p5->translate(0.5f, 0, -4);
    p5->scale(1.5f);
    p5->set_materials(chalk_black, mirror_blue);
    scene.add_object(p5);

    //   std::shared_ptr<Cone> p6 = std::make_shared<Cone>(Cone());
    //   p6->translate(0, 0, 0);
    //   p6->scale(2);
    //   //p5->rotate_x(45);
    //   p6->set_material(mat_red);
    //   scene.add_object(p6);

    std::shared_ptr<Cube> p7 = std::make_shared<Cube>(Cube());
    p7->rotate_y(45);
    p7->rotate_x(45);
    p7->translate(-2.5, 1, -2);
    p7->scale(1.f);
    p7->set_materials(metal_green, mirror_blue);
    scene.add_object(p7);


    std::shared_ptr<FCone> cone = std::make_shared<FCone>(FCone());
    cone->translate(-1.5f, -1, -2);
    //cone->rotate_x(90);
    cone->set_materials(metal_green, chalk_blue);
    cone->scale(2.0f);
    scene.add_object(cone);

    std::shared_ptr<Tore> tore = std::make_shared<Tore>(Tore());
    tore->translate(-1.5f, -1.0f, 0.0f);
    tore->set_materials(chalk_red, chalk_grey);
    tore->scale(1.0f);
    scene.add_object(tore);

    std::shared_ptr<Circle> circle = std::make_shared<Circle>(Circle());
    circle->scale(1.0f);
    circle->translate(0.0f, 0.0f, 0.0f);
    circle->set_materials(chalk_red, chalk_grey);
    scene.add_object(circle);


    std::shared_ptr<Light> l = std::make_shared<Light>(Light(2, -1.5, 0));
    l->color = Color(1.f, 1.f, 0.8f);
    scene.add_light(l);

    std::shared_ptr<Light> l1 = std::make_shared<Light>(Light(-2, -1.5, 0));
    l1->color = Color(1.f, 0.2f, 0.8f);
    scene.add_light(l1);

    // SETTING CAMERA
    Camera test;
    test.focal = 2;
    test.translate(0, 0, 5);
    //test.rotate_x(-30);
    scene.set_camera(test);


    // SET META DATA
    scene.set_bg(Color(0.f, 0.f, 0.f));
    scene.set_ambiant(Color(0.5, 0.5, 0.5));

    std::cout << "Scene is loaded with " << scene.nb_objects() << " objects and "
        << scene.nb_lights() << " lights\n";


    Renderer renderer;
    Camera cam = scene.get_camera();

    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            float x = static_cast<float>(j) / size, y = static_cast<float>(i) / size;

            Ray ray = cam.get_ray(x, y);
            Vector impact;

            const auto pixel_color_a = scene.cast_ray(ray, impact, renderer, 1);

            scene.image[i][j] = pixel_color_a;
        }
    }

    Color** antialiased_image = scene.get_final_image();

    std::ofstream file_ppm;
    file_ppm.open("render.ppm");

    file_ppm << "P3\n" << scene_size << " " << scene_size << "\n255\n";

    for (int i = 0; i < scene_size; i++)
    {
        for (int j = 0; j < scene_size; j++)
        {
            file_ppm << antialiased_image[i][j].r * 255 << " " << antialiased_image[i][j].g * 255 << " " << antialiased_image[i][j].b * 255 << " ";
        }

        file_ppm << "\n";
    }

    for (int i = 0; i < scene_size; i++)
        delete antialiased_image[i];

    delete[] antialiased_image;
    //std::cout << "Res = " << res << std::endl;
    std::cout << "DONE !\n";

    getchar();
    return 0;
}
