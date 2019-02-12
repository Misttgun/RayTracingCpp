//
// Created by florian on 12/02/19.
//

#include <SceneLoader.h>
#include <Scene.h>
#include <json.hpp>
#include <fstream>
#include <iostream>

#include "SceneLoader.h"

using json = nlohmann::json;

Scene * SceneLoader::load(const std::string &file) {
    Scene *scene = new Scene();

    std::ifstream i(file);
    json j;
    i >> j;

    std::cout << j["globals"] << std::endl;

    return scene;
}
