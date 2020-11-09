//
// Created by aurailus on 14/12/18.
//

#pragma once

#include "World.h"

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
	
	void commitChunk(std::shared_ptr<Chunk> chunk);
	
	virtual DimensionPtr
	createDimension(const std::string& identifier, std::unordered_set<std::string>& biomes) override;
	
	DimensionPtr getActiveDimension();
	
	void setActiveDimension(DimensionPtr);
	
	PlayerPtr getPlayer();
	
	virtual InventoryRefsPtr getRefs() override;
	
	ClientNetworkInterpreter& getNet();
	
	int renderChunks(Renderer& render);
	
	void renderEntities(Renderer& renderer);
	
	int mapBlocksInterpolated = 0;
	int lastMeshUpdates = 0;
	private:
	Renderer& renderer;
	
	ClientNetworkInterpreter net;
	std::shared_ptr<LocalInventoryRefs> refs;
	PlayerPtr player;
	
	std::shared_ptr<LocalDimension> activeDimension = nullptr;
	
	std::shared_ptr<WorldInterpolationStream> worldGenStream = nullptr;
};
