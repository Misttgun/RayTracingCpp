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
#include <chrono>
#include <thread>
#include <atomic>
#include <future>

int main()
{
    auto start = std::chrono::steady_clock::now();

    std::shared_ptr<Scene> scene = SceneLoader::load("scene.json");
    const int scene_size = scene->image_size;
    const int size = scene->image_size * scene->_sampling_factor;

    std::cout << "Scene is loaded with " << scene->nb_objects() << " objects and "
        << scene->nb_lights() << " lights\n";


    Renderer renderer;
    Camera cam = scene->get_camera();

    //----------------- PAS DE MULTITHREADING --------------
    /*for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            float x = static_cast<float>(j) / size, y = static_cast<float>(i) / size;

            Ray ray = cam.get_ray(x, y);
            Vector impact;

            const auto pixel_color = scene->cast_ray(ray, impact, renderer, 1);
            scene->image[i][j] = pixel_color;
        }
    }*/

    //------------- ZONE MULTITHREADING ------------------
    std::size_t max = size * size;
    std::size_t cores = std::thread::hardware_concurrency();
    volatile std::atomic<std::size_t> count(0);
    std::vector<std::future<void>> future_vector;

    for (std::size_t k(0); k < cores; ++k)
    {
        future_vector.emplace_back(std::async([=, &scene]()
        {
            for (std::size_t index(k); index < max; index += cores)
            {
                std::size_t i = index / size;
                std::size_t j = index % size;

                float x = static_cast<float>(j) / size;
                float y = static_cast<float>(i) / size;

                Ray ray = cam.get_ray(x, y);
                Vector impact;

                const auto pixel_color_a = scene.cast_ray(ray, impact, renderer, 1);

                scene.image[i][j] = pixel_color_a;
            }
        }));
    }

    for(int i = 0; i < future_vector.size(); i++)
    {
        future_vector.at(i).get();
    }

    //------------- FIN ZONE MULTITHREADING ------------------

    Color** antialiased_image = scene->get_final_image();

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

    auto end = std::chrono::steady_clock::now();
    auto diff = end - start;
    std::cout << std::chrono::duration <double, std::milli>(diff).count() << " ms" << std::endl;
    getchar();
    return 0;
}
