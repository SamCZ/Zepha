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
	
	void cleanup() override;
	
private:
	vec<string> perfSections = { "update:mods", "update:world", "update:player", "update:net", "update:chunks",
		"draw:world", "draw:entities", "draw:interface", "update:debug", "idle" };
	PerfTimer perf { perfSections };
	
	WorldPtr world;
};

