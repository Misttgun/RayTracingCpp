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

int sdl_loop(const std::shared_ptr<Scene> &scene);
void render_image_sdl(Color **image, SDL_Renderer *sdl_renderer, int size);
void save_scene_to_ppm(const std::shared_ptr<Scene> &scene);
void save_scene_to_bmp(const std::shared_ptr<Scene> &scene);

int main()
{
    std::string scene_name;
    std::string file_name;
    bool do_real_time_display;

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

    std::cout << "Do you want real time display? (true/false) ";
    if (not (std::cin >> std::boolalpha >> do_real_time_display))
    {
        std::cout << "Did not understand your choice, assuming false."
            << std::endl;
        do_real_time_display = false;
    }

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                     "Couldn't initialize SDL: %s", SDL_GetError());
        return 3;
    }

    const auto start = std::chrono::steady_clock::now();

    const int size = scene->image_size * scene->_sampling_factor;
    scene->output_file = file_name;

    std::cout << "Scene is loaded with " << scene->nb_objects() << " objects and " << scene->nb_lights() << " lights\n";

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
    std::vector<std::future<void>> future_vector;

    for (std::size_t k(0); k < cores; ++k)
    {
        future_vector.emplace_back(std::async([=, &scene]()
        {
            for (std::size_t index(k); index < max; index += cores)
            {
                const std::size_t i = index / size;
                const std::size_t j = index % size;

                const float x = static_cast<float>(j) / size;
                const float y = static_cast<float>(i) / size;

                Ray ray = cam.get_ray(x, y);
                Vector impact;

                const auto pixel_color_a = scene->cast_ray(ray, impact, renderer, 1);

                scene->image[i][j] = pixel_color_a;
            }
        }));
    }

    if (do_real_time_display)
    {
        const int sdl_loop_result = sdl_loop(scene);

        if (sdl_loop_result != 0)
        {
            std::cerr << "SDL loop failed, returning " << sdl_loop_result
                << std::endl;
            return sdl_loop_result;
        }
    }

    for (auto& i : future_vector)
    {
        i.get();
    }

    //------------- FIN ZONE MULTITHREADING ------------------

    std::cout << "Now saving antialiased image..." << std::endl;

    save_scene_to_ppm(scene);
    save_scene_to_bmp(scene);

    //std::cout << "Res = " << res << std::endl;
    std::cout << "DONE !\n";

    const auto end = std::chrono::steady_clock::now();
    const auto diff = end - start;
    std::cout << std::chrono::duration <double, std::milli>(diff).count() << " ms" << std::endl;
    getchar();
    return 0;
}

/**
 * Initializes SDL and displays rendered image in a window
 * @param scene The scene to display in the window
 * @return 0 on success, not 0 on error
 */
int sdl_loop(const std::shared_ptr<Scene> &scene)
{
    const int scene_size = scene->image_size;
    SDL_Event event;
    SDL_Window* window = SDL_CreateWindow("Ray Tracing", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, scene_size, scene_size, 0);
    SDL_Renderer* sdl_renderer = SDL_CreateRenderer(window, -1, 0);

    bool stop = false;
    while (!stop)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                std::cout << "Stopping real time rendering." << std::endl;
                stop = true;
                break;
            }
        }

        Color** antialiased_image = scene->get_final_image();

        render_image_sdl(antialiased_image, sdl_renderer, scene_size);

        SDL_Delay(100);
    }

    SDL_DestroyRenderer(sdl_renderer);
    SDL_Quit();

    return 0;
}

/**
 * Renders passed image using passed SDL_Renderer
 * @param image The image to be rendered
 * @param sdl_renderer The renderer to use for rendering the image
 * @param scene_size The size of a side of the image
 */
void render_image_sdl(Color **image, SDL_Renderer *sdl_renderer,
                      const int scene_size)
{
    for (int i = 0; i < scene_size; i++)
    {
        for (int j = 0; j < scene_size; j++)
        {
            const Color color = image[i][j];
            SDL_SetRenderDrawColor(sdl_renderer,
                                   static_cast<Uint8> (color.r * 255),
                                   static_cast<Uint8> (color.g * 255),
                                   static_cast<Uint8> (color.b * 255), 255);
            SDL_RenderDrawPoint(sdl_renderer, j, i);
        }
    }

    SDL_RenderPresent(sdl_renderer);
}

/**
 * Saves the final image of passed scene in its output file, BMP format
 * @param scene The scene of which the image should be saved in a BMP file
 */
void save_scene_to_bmp(const std::shared_ptr<Scene> &scene)
{
    std::string file_name = scene->output_file;
    Color **image = scene->get_final_image();

    file_name.append(".bmp");

    SDL_Surface* surface = SDL_CreateRGBSurface(0, scene->image_size, scene->image_size, 32, 0, 0, 0, 0);

    if (surface == nullptr)
    {
        std::cerr << "SDL_CreateRGBSurface() failed: %s" << SDL_GetError()
            << std::endl;
        exit(1);
    }

    SDL_Renderer *sdl_renderer = SDL_CreateSoftwareRenderer(surface);

    render_image_sdl(image, sdl_renderer, scene->image_size);

    SDL_SaveBMP(surface, file_name.c_str());
}

/**
 * Saves the final image of passed scene in its output file, PPM format
 * @param scene The scene of which the image should be saved in a PPM file
 */
void save_scene_to_ppm(const std::shared_ptr<Scene> &scene)
{
    const int scene_size = scene->image_size;
    std::string file_name = scene->output_file;
    Color** antialiased_image = scene->get_final_image();

    file_name.append(".ppm");

    std::ofstream file_ppm;
    file_ppm.open(file_name);

    file_ppm << "P3\n" << scene_size << " " << scene_size << "\n255\n";

    for (int i = 0; i < scene_size; i++)
    {
        for (int j = 0; j < scene_size; j++)
        {
            file_ppm << antialiased_image[i][j].r * 255 << " " << antialiased_image[i][j].g * 255 << " " << antialiased_image[i][j].b * 255 << " ";
        }

        file_ppm << "\n";
    }

    file_ppm.close();
}
