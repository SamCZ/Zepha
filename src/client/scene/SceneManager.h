#pragma once

#include "Scene.h"
#include "util/Types.h"

/**
 * Handles updating and switching scenes.
 */

class SceneManager {
public:
	
	/** Sets the current scene to the one provided. */
	void setScene(uptr<Scene> scene);
	
	bool isCurrent(Scene* self);
	
	/** Updates the current scene. */
	void update();
	
	~SceneManager();
	
private:
	
	/** Cleans up the current scene, and removes it. */
	void cleanupScene();
	
	/** A pointer to the current scene. */
	std::unique_ptr<Scene> scene = nullptr;
};

