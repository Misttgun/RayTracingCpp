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

using json = nlohmann::json;

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
    //i >> j;

    std::shared_ptr<Scene> scene = loadGlobals(j);
    std::vector<Material> materials = loadMaterials(j);
    loadObjects(scene, j, materials);
    loadLights(scene, j);

    return scene;
}

std::shared_ptr<Scene>  SceneLoader::loadGlobals(nlohmann::basic_json<> json)
{
    std::cout << "Loading global settings." << std::endl;
    auto globals = json["globals"];
    int image_size = globals["image_size"];
    unsigned int sampling_factor = globals["sampling_factor"];

    std::shared_ptr<Scene> scene = std::make_shared<Scene>(Scene(image_size, sampling_factor));
    //std::string test = globals["output_file"];;
    //scene->output_file = test;
    scene->apply_shadows = globals["apply_shadows"];

    auto camera_data = globals["camera"];
    float x = camera_data["x"];
    float y = camera_data["y"];
    float z = camera_data["z"];
    float focal = camera_data["focal"];

    const auto camera = Camera(x, y, z, focal);
    scene->set_camera(camera);

    auto background_color_data = globals["background_color"];
    float bg_r = background_color_data["r"];
    float bg_g = background_color_data["g"];
    float bg_b = background_color_data["b"];
    scene->set_bg(Color(bg_r, bg_g, bg_b));

    auto ambiant_color_data = globals["ambiant_color"];
    float ambiant_r = ambiant_color_data["r"];
    float ambiant_g = ambiant_color_data["g"];
    float ambiant_b = ambiant_color_data["b"];
    scene->set_ambiant(Color(ambiant_r, ambiant_g, ambiant_b));

    return scene;
}

std::vector<Material> SceneLoader::loadMaterials(nlohmann::basic_json<> json)
{
    std::cout << "Loading materials." << std::endl;
    std::vector<Material> materials;

    for (const auto &material_data : json["materials"])
    {
        auto color_data = material_data["color"];
        float r = color_data["r"];
        float g = color_data["g"];
        float b = color_data["b"];
        Color color(r, g, b);

        float kd = material_data["kd"];
        float ks = material_data["ks"];
        float shininess = material_data["shininess"];
        float light_influence = material_data["light_influence"];
        Type type = material_data["type"];

        Material material(color, kd, ks, shininess, light_influence, type);
        materials.push_back(material);
    }

    return materials;
}

void SceneLoader::loadObjects(std::shared_ptr<Scene> scene, nlohmann::basic_json<> json, std::vector<Material> materials)
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
        else
        {
            std::cerr << "Invalid object type: " << type << std::endl;
            continue;
        }

        if (object_data.find("rotation") != object_data.end())
        {
            auto rotation_data = object_data["rotation"];
            float x = rotation_data["x"];
            float y = rotation_data["y"];
            float z = rotation_data["z"];
            object->rotate_x(x);
            object->rotate_y(y);
            object->rotate_z(z);
        }

        auto translation_data = object_data["translation"];
        float x = translation_data["x"];
        float y = translation_data["y"];
        float z = translation_data["z"];
        object->translate(x, y, z);

        if (object_data.find("scale") != object_data.end())
        {
            float scale = object_data["scale"];
            object->scale(scale);
        }

        int material_index = object_data["material"];
        if (object_data.find("material_2") != object_data.end())
        {
            int material_index_2 = object_data["material_2"];
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

void SceneLoader::loadLights(std::shared_ptr<Scene> scene, nlohmann::basic_json<> json)
{
    std::cout << "Loading lights." << std::endl;
    for (const auto &light_data : json["lights"])
    {
        std::shared_ptr<Light> light = std::make_shared<Light>(Light());

        auto translation_data = light_data["translation"];
        float x = translation_data["x"];
        float y = translation_data["y"];
        float z = translation_data["z"];
        light->translate(x, y, z);

        auto color_data = light_data["color"];
        float r = color_data["r"];
        float g = color_data["g"];
        float b = color_data["b"];
        Color color(r, g, b);
        light->color = color;

        scene->add_light(light);
    }
}

