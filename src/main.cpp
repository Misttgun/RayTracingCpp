#include "Scene.h"
#include "Vector.h"
#include "Renderer.h"

#include <iostream>
#include <memory>
#include <fstream>
#include <SceneLoader.h>
#include <chrono>
#include <thread>
#include <atomic>
#include <future>
#include <SDL.h>

#undef main

int main()
{
    SDL_Event event;

    std::string scene_name;
    std::string file_name;

    std::cout << "Write the scene file name : ";
    std::cin >> scene_name;
    scene_name.append(".json");

    const std::string scene_file_path = "../res/" + scene_name;
    std::shared_ptr<Scene> scene = SceneLoader::load(scene_file_path);

    //If scene is not loaded
    if (!scene)
        return 1;

    std::cout << "Write the output file name : ";
    std::cin >> file_name;
    file_name.append(".ppm");

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s", SDL_GetError());
        return 3;
    }

    const auto start = std::chrono::steady_clock::now();

    const int scene_size = scene->image_size;
    const int size = scene->image_size * scene->_sampling_factor;
    scene->output_file = file_name;

    std::cout << "Scene is loaded with " << scene->nb_objects() << " objects and " << scene->nb_lights() << " lights\n";

    SDL_Window* window = SDL_CreateWindow("Ray Tracing", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, scene_size, scene_size, 0);
    SDL_Renderer* sdl_renderer = SDL_CreateRenderer(window, -1, 0);

    const Renderer renderer;
    const Camera cam = scene->get_camera();

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
    const std::size_t max = size * size;
    const std::size_t cores = std::thread::hardware_concurrency();
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

                const auto pixel_color_a = scene->cast_ray(ray, impact, renderer, 1);

                scene->image[i][j] = pixel_color_a;
            }
        }));
    }

    const auto sdl_start = std::chrono::steady_clock::now();

    while (true)
    {
        SDL_PollEvent(&event);
        if (event.type == SDL_QUIT)
        {
            break;
        }

        Color** antialiased_image = scene->get_final_image();

        for (int i = 0; i < scene_size; i++)
        {
            for (int j = 0; j < scene_size; j++)
            {
                const Color color = antialiased_image[i][j];
                SDL_SetRenderDrawColor(sdl_renderer,
                                       static_cast<Uint8> (color.r * 255),
                                       static_cast<Uint8> (color.g * 255),
                                       static_cast<Uint8> (color.b * 255), 255);
                SDL_RenderDrawPoint(sdl_renderer, j, i);
            }
        }

        SDL_RenderPresent(sdl_renderer);

        SDL_Delay(100);
    }

    const auto sdl_end = std::chrono::steady_clock::now();

    std::cout << "Now saving antialiased image..." << std::endl;

    for (auto& i : future_vector)
    {
        i.get();
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

    SDL_DestroyRenderer(sdl_renderer);
    SDL_Quit();


    //std::cout << "Res = " << res << std::endl;
    std::cout << "DONE !\n";

    const auto end = std::chrono::steady_clock::now();
    const auto diff = end - start - (sdl_end - sdl_start);
    std::cout << std::chrono::duration <double, std::milli>(diff).count() << " ms" << std::endl;
    getchar();
    return 0;
}
