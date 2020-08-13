//
// Created by aurailus on 06/01/19.
//

#include "SceneManager.h"

void SceneManager::update() {
    scene->update();
    scene->draw();
}

void SceneManager::setScene(std::unique_ptr<Scene> scene) {
    if (this->scene != nullptr) cleanupScene();
    this->scene = std::move(scene);
}

Scene& SceneManager::getScene() {
    return *scene;
}

void SceneManager::cleanupScene() {
    if (scene != nullptr) {
        scene->cleanup();
        scene = nullptr;
    }
}

SceneManager::~SceneManager() {
    cleanupScene();
}