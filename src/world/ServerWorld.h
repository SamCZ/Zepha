//
// World subclass for the server.
// Handles blocks, entities, and clients.
// Created by aurailus on 05/03/19.
//

#pragma once

#include "world/World.h"

#include "world/dim/ServerDimension.h"

class Target;

class ItemStack;

class ServerClients;

class ServerPlayer;

class ServerInventoryRefs;

class ServerSubgame;

class ServerGenStream;

class ServerPacketStream;

class ServerWorld : public World {
	public:
	explicit ServerWorld(unsigned int seed, SubgamePtr game, ServerClients& clients);
	
	void init(const std::string& worldDir);
	
	void update(double delta) override;
	
	virtual void sendMessage(const std::string& channel, const std::string& message) override;
	
	virtual DimensionPtr
	createDimension(const std::string& identifier, std::unordered_set<std::string>& biomes) override;
	
	virtual DimensionPtr getDimension(unsigned int index) override;
	
	virtual DimensionPtr getDimension(const std::string& identifier) override;
	
	virtual InventoryRefsPtr getRefs() override;
	
	virtual ServerClients& getClients();
	
	private:
	void changedMapBlocks(ServerPlayer& player);
	
	bool generateMapBlock(unsigned int dim, glm::ivec3 pos);
	
	void generateMapBlocks(ServerPlayer& player);
	
	void sendChunksToPlayer(ServerPlayer& client);
	
	std::shared_ptr<ServerGenStream> genStream = nullptr;
	std::shared_ptr<ServerPacketStream> packetStream = nullptr;
	
	unsigned int seed;
	ServerClients& clients;
	std::shared_ptr<ServerInventoryRefs> refs;

//    std::string worldDir;
//    std::shared_ptr<FileManipulator> fileManip;
	
	unsigned int generatedMapBlocks = 0;
	std::vector<glm::ivec3> generateOrder;
	
	const glm::ivec2 mapBlockGenRange = { 4, 4 };
	const glm::ivec2 sendRange = { 4, 4 };
	const glm::ivec2 activeChunkRange = { 16, 16 };
};

