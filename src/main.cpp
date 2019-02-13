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
#include <SceneLoader.h>
#include "Cube.h"
#include "Cone.h"
#include "Tore.h"

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

    std::shared_ptr<Scene> scene = SceneLoader::load("scene.json");

    auto size = scene->image_size;

    std::cout << "Scene is loaded with " << scene->nb_objects() << " objects and "
        << scene->nb_lights() << " lights\n";


    Renderer renderer;
    Camera cam = scene->get_camera();

    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            float x = static_cast<float>(j) / size, y = static_cast<float>(i) / size;

            Ray ray = cam.get_ray(x, y);
            Vector impact;

            const auto pixel_color = scene->cast_ray(ray, impact, renderer, 1);
            scene->image[i][j] = pixel_color;
        }
    }

    std::ofstream file_ppm;
    file_ppm.open(scene->output_file);

    file_ppm << "P3\n" << size << " " << size << "\n255\n";

    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            file_ppm << scene->image[i][j].r * 255 << " " << scene->image[i][j].g * 255 << " " << scene->image[i][j].b * 255 << " ";
        }

        file_ppm << "\n";
    }
    //std::cout << "Res = " << res << std::endl;
    std::cout << "DONE !\n";

    getchar();
    return 0;
}
