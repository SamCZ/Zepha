//
// Created by aurailus on 06/01/19.
//

#pragma once

#include <memory>

#include "Scene.h"

class SceneManager {
public:
    void setScene(std::unique_ptr<Scene> scene);
    const Scene& getScene();

    void update();
    void cleanupScene();

    ~SceneManager();

private:
    std::unique_ptr<Scene> scene = nullptr;
};

