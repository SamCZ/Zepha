#pragma once

#include "Scene.h"

#include "util/PerfTimer.h"
#include "world/LocalWorld.h"

class Drawable;
class LocalSubgame;

class GameScene : public Scene {
public:
	
	GameScene(Client& client);
	
	void update() override;
	
	void draw() override;
	
private:
	vec<string> perfSections = { "update:world", "update:player", "update:debug",
		"draw:world", "draw:entities", "draw:interface", "other", "idle" };
	PerfTimer perf { perfSections };
	
	WorldPtr world;
};

