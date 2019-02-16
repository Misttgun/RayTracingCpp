//
// Created by florian on 12/02/19.
//

#ifndef RAYTRACING_SCENELOADER_H
#define RAYTRACING_SCENELOADER_H

#include <json.hpp>
#include "Scene.h"

class SceneLoader
{
public:
    static std::shared_ptr<Scene> load(const std::string& file);

private:
    SceneLoader() = default;

    static std::shared_ptr<Scene> loadGlobals(nlohmann::basic_json<> json);
    static void loadLights(std::shared_ptr<Scene> scene, nlohmann::basic_json<> json);
    static std::vector<Material> loadMaterials(nlohmann::basic_json<> json);
    static void loadObjects(std::shared_ptr<Scene> scene, nlohmann::basic_json<> json, std::vector<Material> materials);
};


#endif //RAYTRACING_SCENELOADER_H
