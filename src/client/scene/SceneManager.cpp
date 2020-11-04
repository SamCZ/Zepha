//
// Created by aurailus on 06/01/19.
//

#include "SceneManager.h"


/**
 * Sets a new scene as the current scene.
 * Cleans up the old scene.
 * @param newScene - The new current scene.
 */

void SceneManager::setScene(std::unique_ptr<Scene> newScene) {
    cleanupScene();
    scene = std::move(newScene);
}


/**
 * Returns a reference to the current scene.
 * @returns The current scene as a const reference.
 */

const Scene& SceneManager::getScene() {
    return *scene;
}


/**
 * Updates and renders the current scene, if there is one.
 */

void SceneManager::update() {
    if (!scene) return;
    scene->update();
    scene->draw();
}


/**
 * Cleans up the current scene and removes it.
 * There will be no current scene after this method executes.
 */

void SceneManager::cleanupScene() {
    if (scene) scene->cleanup();
    scene = nullptr;
}


/*
 * Clean up the scene on destruction.
 */

SceneManager::~SceneManager() {
    cleanupScene();
}