//
// Created by florian on 12/02/19.
//

#include <SceneLoader.h>
#include <Scene.h>
#include <json.hpp>
#include <fstream>
#include <iostream>
#include <Plan.h>
#include <Square.h>
#include <Sphere.h>
#include <Cone.h>
#include <Cube.h>
#include <Cylinder.h>
#include <Tore.h>
#include "FCone.h"
#include "FCylinder.h"
#include "Circle.h"
#include "Triangle.h"

using json = nlohmann::json;

/**
 * Loads a scene from passed file path and returns it
 * @param file The file path from which to load the scene
 * @return A pointer to the newly allocated and loaded scene
 */
std::shared_ptr<Scene> SceneLoader::load(const std::string &file)
{
    std::ifstream i(file);
    if (!i.is_open())
    {
        std::cout << "Incorrect scene name..." << std::endl;
        return nullptr;
    }

    json j;
    try
    {
        j = json::parse(i);
    }
    catch (json::parse_error &e)
    {
        std::cerr << e.what() << std::endl;
    }

    std::shared_ptr<Scene> scene = loadGlobals(j);
    const std::vector<Material> materials = loadMaterials(j);
    loadObjects(scene, j, materials);
    loadLights(scene, j);

    return scene;
}

/**
 * Instantiates the scene and loads scene global settings from JSON
 * @param json The configuration JSON object
 * @return A pointer to the newly allocated
 */
std::shared_ptr<Scene>  SceneLoader::loadGlobals(nlohmann::basic_json<> json)
{
    std::cout << "Loading global settings." << std::endl;
    auto globals = json["globals"];
    const int image_size = globals["image_size"];
    const unsigned int sampling_factor = globals["sampling_factor"];

    std::shared_ptr<Scene> scene =
            std::make_shared<Scene>(Scene(image_size, sampling_factor));

    scene->apply_shadows = globals["apply_shadows"];

    auto camera_data = globals["camera"];
    const float x = camera_data["x"];
    const float y = camera_data["y"];
    const float z = camera_data["z"];
    const float focal = camera_data["focal"];

    const auto camera = Camera(x, y, z, focal);
    scene->set_camera(camera);

    auto background_color_data = globals["background_color"];
    const float bg_r = background_color_data["r"];
    const float bg_g = background_color_data["g"];
    const float bg_b = background_color_data["b"];
    scene->set_bg(Color(bg_r, bg_g, bg_b));

    auto ambiant_color_data = globals["ambiant_color"];
    const float ambiant_r = ambiant_color_data["r"];
    const float ambiant_g = ambiant_color_data["g"];
    const float ambiant_b = ambiant_color_data["b"];
    scene->set_ambiant(Color(ambiant_r, ambiant_g, ambiant_b));

    return scene;
}

/**
 * Loads the materials from the JSON configuration
 * @param json The JSON configuration object
 * @return A vector containing all the loaded materials
 */
std::vector<Material> SceneLoader::loadMaterials(nlohmann::basic_json<> json)
{
    std::cout << "Loading materials." << std::endl;
    std::vector<Material> materials;

    for (const auto &material_data : json["materials"])
    {
        auto color_data = material_data["color"];
        const float r = color_data["r"];
        const float g = color_data["g"];
        const float b = color_data["b"];
        Color color(r, g, b);

        const float kd = material_data["kd"];
        const float ks = material_data["ks"];
        const float shininess = material_data["shininess"];
        const float light_influence = material_data["light_influence"];
        const Type type = material_data["type"];

        Material material(color, kd, ks, shininess, light_influence, type);
        materials.push_back(material);
    }

    return materials;
}

/**
 * Loads the scene objects from JSON configuration
 * @param scene The scene to populate with the loaded objects
 * @param json The JSON configuration object
 * @param materials The materials previously loaded
 */
void SceneLoader::loadObjects(const std::shared_ptr<Scene>& scene, nlohmann::basic_json<> json, std::vector<Material> materials)
{
    std::cout << "Loading objects." << std::endl;
    for (const auto &object_data : json["objects"])
    {
        auto type = object_data["type"];
        std::shared_ptr<Object> object = nullptr;

        if (type == "plan")
        {
            object = std::make_shared<Plan>(Plan());
        }
        else if (type == "square")
        {
            object = std::make_shared<Square>(Square());
        }
        else if (type == "sphere")
        {
            object = std::make_shared<Sphere>(Sphere());
        }
        else if (type == "cone")
        {
            object = std::make_shared<Cone>(Cone());
        }
        else if (type == "fcone")
        {
            object = std::make_shared<FCone>(FCone());
        }
        else if (type == "cube")
        {
            object = std::make_shared<Cube>(Cube());
        }
        else if (type == "circle")
        {
            object = std::make_shared<Circle>(Circle());
        }
        else if (type == "cylinder")
        {
            object = std::make_shared<Cylinder>(Cylinder());
        }
        else if (type == "fcylinder")
        {
            object = std::make_shared<FCylinder>(FCylinder());
        }
        else if (type == "tore")
        {
            object = std::make_shared<Tore>(Tore());
        }
        else if (type == "triangle")
        {
            object = std::make_shared<Triangle>(Triangle());
        }
        else
        {
            std::cerr << "Invalid object type: " << type << std::endl;
            continue;
        }

        if (object_data.find("rotation") != object_data.end())
        {
            auto rotation_data = object_data["rotation"];
            const float x = rotation_data["x"];
            const float y = rotation_data["y"];
            const float z = rotation_data["z"];
            object->rotate_x(x);
            object->rotate_y(y);
            object->rotate_z(z);
        }

        auto translation_data = object_data["translation"];
        const float x = translation_data["x"];
        const float y = translation_data["y"];
        const float z = translation_data["z"];
        object->translate(x, y, z);

        if (object_data.find("scale") != object_data.end())
        {
            const float scale = object_data["scale"];
            object->scale(scale);
        }

        const int material_index = object_data["material"];
        if (object_data.find("material_2") != object_data.end())
        {
            const int material_index_2 = object_data["material_2"];
            object->set_materials(materials[material_index],
                                  materials[material_index_2]);
        }
        else
        {
            object->set_material(materials[material_index]);
        }

        scene->add_object(object);
    }
};

/**
 * Loads the scene lights from the JSON configuration
 * @param scene The scene to populate with the loaded lights
 * @param json The JSON configuration object
 */
void SceneLoader::loadLights(const std::shared_ptr<Scene>& scene, nlohmann::basic_json<> json)
{
    std::cout << "Loading lights." << std::endl;
    for (const auto &light_data : json["lights"])
    {
        std::shared_ptr<Light> light = std::make_shared<Light>(Light());

        auto translation_data = light_data["translation"];
        const float x = translation_data["x"];
        const float y = translation_data["y"];
        const float z = translation_data["z"];
        light->translate(x, y, z);

        auto color_data = light_data["color"];
        const float r = color_data["r"];
        const float g = color_data["g"];
        const float b = color_data["b"];
        const Color color(r, g, b);
        light->color = color;

        scene->add_light(light);
    }
}

