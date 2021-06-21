//
// Created by aurailus on 14/12/18.
//

#pragma once

#include "World.h"

#include "client/gui/DebugGui.h"
#include "world/dim/LocalDimension.h"
#include "client/conn/ClientNetworkInterpreter.h"

class Window;
class Renderer;
class LocalPlayer;
class LocalSubgame;
class LocalInventoryRefs;
class WorldInterpolationStream;

class LocalWorld : public World {
public:
	LocalWorld(SubgamePtr game, ServerConnection& conn, Renderer& window);
	
	void connect();
	
	bool updatePlayerDimension();
	
	void update(double delta) override;
	
	void handleWorldPacket(std::unique_ptr<PacketView> p);
	
	void handlePlayerEntPacket(std::unique_ptr<PacketView> p);
	
	void handleModMessage(const std::string& channel, const std::string& message);
	
	void commitChunk(std::shared_ptr<Chunk> chunk);
	
	virtual void sendMessage(const std::string& channel, const std::string& message) override;
	
	virtual DimensionPtr
	createDimension(const std::string& identifier, std::unordered_set<std::string>& biomes) override;
	
	DimensionPtr getActiveDimension();
	
	void setActiveDimension(DimensionPtr);
	
	PlayerPtr getPlayer();
	
	virtual InventoryRefsPtr getRefs() override;
	
	ClientNetworkInterpreter& getNet();
	
	/** Renders the visible block chunks to the screen. */
	void drawWorld();
	/** Renders the visible entities to the screen. */
	void drawEntities();
	/** Renders non-diagetic (UI) elements to the screen using an orthographic projection. */
	void drawInterface();
	
private:
	Renderer& renderer;
	
	ClientNetworkInterpreter net;
	std::shared_ptr<LocalInventoryRefs> refs;
	PlayerPtr player;
	
	DebugGui debugGui;
	uint32_t lastInterpolations = 0;
	
	bool hudVisible = true;
	bool debugVisible = true;
	
	std::shared_ptr<LocalDimension> activeDimension = nullptr;
	std::shared_ptr<WorldInterpolationStream> worldGenStream = nullptr;
};
