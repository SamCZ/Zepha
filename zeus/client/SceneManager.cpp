//
// Created by aurailus on 06/01/19.
//

#include "SceneManager.h"

SceneManager::SceneManager() {
    scene = nullptr;
};

void SceneManager::update() {
    scene->update();
    scene->draw();
}

void SceneManager::setScene(Scene* scene) {
    if (this->scene != nullptr) cleanupScene();
    this->scene = scene;
}

Scene* SceneManager::getScene() {
    return scene;
}

void SceneManager::cleanupScene() {
    scene->cleanup();
    delete scene;
    scene = nullptr;
}

SceneManager::~SceneManager() {
    if (scene != nullptr) cleanupScene();
}