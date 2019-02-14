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
#include <SceneLoader.h>
#include "Cube.h"
#include "Cone.h"
#include "FCone.h"
#include "Tore.h"
#include "Circle.h"

int main()
{
    std::shared_ptr<Scene> scene = SceneLoader::load("scene.json");
    const int scene_size = scene->image_size;
    const int size = scene->image_size * scene->_sampling_factor;

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

    Color** antialiased_image = scene.get_final_image();

    std::ofstream file_ppm;
    file_ppm.open(scene->output_file);

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
