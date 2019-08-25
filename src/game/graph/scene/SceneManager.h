//
// Created by aurailus on 06/01/19.
//

#pragma once

#include "Scene.h"
#include <memory>

class SceneManager {
public:
    SceneManager() = default;

    void setScene(std::unique_ptr<Scene> scene);
    Scene& getScene();

    void update();
    void cleanupScene();

    ~SceneManager();
private:
    std::unique_ptr<Scene> scene = nullptr;
};

