//
// Created by aurailus on 27/12/18.
//

#pragma once

#include "client/gui/basic/GuiContainer.h"

#include "util/CovariantPtr.h"

class LocalPlayer;

class LocalSubgame;

class LocalWorld;

class DebugGui : public GuiContainer {
	public:
	DebugGui(glm::vec2 bufferSize, SubgamePtr game, WorldPtr world);
	
	void bufferResized(glm::vec2 bufferSize);
	
	void changeVisibilityState(int state);
	
	void positionElements(glm::vec2 bufferSize);
	
	void update(std::shared_ptr<LocalPlayer> player, double fps, int chunks, int drawCalls, int ssGen, int ssPack);
	
	private:
	int displayMode;
	
	WorldPtr world;
	SubgamePtr game;
};
