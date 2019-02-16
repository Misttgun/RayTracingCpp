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
    SDL_Window *window;
    SDL_Renderer *sdl_renderer;
    SDL_Texture *texture;
    SDL_Event event;

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                     "Couldn't initialize SDL: %s", SDL_GetError());
        return 3;
    }

    auto start = std::chrono::steady_clock::now();

    std::shared_ptr<Scene> scene = SceneLoader::load("../res/scene.json"); //TODO Modifier pour permettre � l'utilisateur de choisir sa sc�ne
    //std::shared_ptr<Scene> scene = SceneLoader::load("../../res/scene.json");
    const int scene_size = scene->image_size;
    const int size = scene->image_size * scene->_sampling_factor;

    std::cout << "Scene is loaded with " << scene->nb_objects() << " objects and "
        << scene->nb_lights() << " lights\n";

    window = SDL_CreateWindow("Ray Tracing", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, scene_size, scene_size, 0);
    sdl_renderer = SDL_CreateRenderer(window, -1, 0);
    texture = SDL_CreateTexture(sdl_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, scene_size, scene_size);


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

                const auto pixel_color_a = scene->cast_ray(ray, impact, renderer, 1);

                scene->image[i][j] = pixel_color_a;
            }
        }));
    }

    while (true)
    {
        Color color;
        SDL_PollEvent(&event);
        if (event.type == SDL_QUIT)
        {
            break;
        }

        Color** antialiased_image = scene->get_final_image();

        //SDL_SetRenderTarget(sdl_renderer, texture);

        for (int i = 0; i < scene_size; i++)
        {
            for (int j = 0; j < scene_size; j++)
            {
                color = antialiased_image[i][j];
                SDL_SetRenderDrawColor(sdl_renderer,
                                       static_cast<Uint8> (color.r * 255),
                                       static_cast<Uint8> (color.g * 255),
                                       static_cast<Uint8> (color.b * 255), 255);
                SDL_RenderDrawPoint(sdl_renderer, j, i);
            }
        }

        //SDL_SetRenderTarget(sdl_renderer, nullptr);

        //SDL_RenderCopy(sdl_renderer, texture, nullptr, nullptr);
        SDL_RenderPresent(sdl_renderer);

        SDL_Delay(100);
    }

    std::cout << "Now saving antialiased image..." << std::endl;

    for (int i = 0; i < future_vector.size(); i++)
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

    SDL_DestroyRenderer(sdl_renderer);
    SDL_Quit();


    //std::cout << "Res = " << res << std::endl;
    std::cout << "DONE !\n";

    auto end = std::chrono::steady_clock::now();
    auto diff = end - start;
    std::cout << std::chrono::duration <double, std::milli>(diff).count() << " ms" << std::endl;
    getchar();
    return 0;
}
