//
// Created by aurailus on 17/12/18.
//

#pragma once

#include "Scene.h"

#include "world/LocalWorld.h"

class LocalSubgame;
class Drawable;

class GameScene : public Scene {
public:
	GameScene(Client& client);
	
	void update() override;
	
	void draw() override;
	
	void cleanup() override;
	
public:
	WorldPtr world;
};

