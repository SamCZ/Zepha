#include "SceneManager.h"

void SceneManager::setScene(uptr<Scene> newScene) {
	cleanupScene();
	scene = std::move(newScene);
}

bool SceneManager::isCurrent(Scene* self) {
	return scene.get() == self;
}

void SceneManager::update() {
	if (!scene) return;
	scene->update();
	scene->draw();
}

void SceneManager::cleanupScene() {
	if (scene) scene->cleanup();
	scene = nullptr;
}

SceneManager::~SceneManager() {
	cleanupScene();
}
