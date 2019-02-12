//
// Created by florian on 12/02/19.
//

#ifndef RAYTRACING_SCENELOADER_H
#define RAYTRACING_SCENELOADER_H


#include <string>
#include "Scene.h"

class SceneLoader {
public:
    static Scene * load(const std::string& file);

private:
    SceneLoader() = default;
};


#endif //RAYTRACING_SCENELOADER_H
