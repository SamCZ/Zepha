//
// Created by aurailus on 27/12/18.
//

#pragma once

#include "client/gui/basic/GuiContainer.h"

#include "util/CovariantPtr.h"

class LocalWorld;
class LocalPlayer;
class LocalSubgame;

class DebugGui : public GuiContainer {
public:
	DebugGui(glm::vec2 bufferSize, SubgamePtr game, LocalWorld& world);
	
	void bufferResized(glm::vec2 bufferSize);
	
	void changeVisibilityState(int state);
	
	void positionElements(glm::vec2 bufferSize);
	
	void update(std::shared_ptr<LocalPlayer> player, double delta,
		uint32_t interpolatedChunks, uint32_t generatedChunks, uint32_t recievedPackets,
		uint32_t drawnMeshChunks, uint32_t generatedMeshChunks);
	
private:
	int displayMode;
	
	LocalWorld& world;
	SubgamePtr game;
};
