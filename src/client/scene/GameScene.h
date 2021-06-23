#pragma once

#include "Scene.h"

#include "world/LocalWorld.h"

class Drawable;
class LocalSubgame;

class GameScene : public Scene {
public:
	
	GameScene(Client& client);
	
	void update() override;
	
	void draw() override;
	
	void cleanup() override;
	
public:
	
	WorldPtr world;
};

