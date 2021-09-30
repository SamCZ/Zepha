#pragma once

#include "world/World.h"

#include "world/dim/ServerDimension.h"

class Target;
class ItemStack;
class ServerPlayer;
class ServerClients;
class ServerSubgame;
class ServerGenStream;
class ServerInventoryRefs;
class ServerPacketStream;

/**
 * Manages server dimensions and players.
 * Handles sending chunk and entity data to clients.
 */

class ServerWorld : public World {
public:
	
	explicit ServerWorld(u32 seed, SubgamePtr game, ServerClients& clients);
	
	/** Initializes the map and packet thread pools. */
	void init(const string& worldDir);
	
	/** Updates dimensions, and sends new or dirty chunks to clients. */
	void update(double delta);

	/** Sends a mod message to the channel provided. */
	virtual void sendMessage(const string& channel, const string& message) override;
	
	/** Creates a new dimension with the identifier and biomes provided. */
	virtual DimensionPtr createDimension(const string& identifier, std::unordered_set<string>& biomes) override;
	
	/** Returns a reference to the world's inventory refs. */
	virtual InventoryRefsPtr getRefs() override;
	
	/** Gets the list of connected clients. */
	virtual ServerClients& getClients();
	
private:
	
	/** Called when a player changes mapblocks, to generate and send new chunks. */
	void changedMapBlocks(ServerPlayer& player);
	
	/** Generates a single mapblock, if it doesn't exist already. */
	bool generateMapBlock(u16 dim, ivec3 pos);
	
	/** Generates mapblocks around the player specified. */
	void generateMapBlocks(ServerPlayer& player);
	
	/** Sends all of the surrounding chunks to the specified player. */
	void sendChunksToPlayer(ServerPlayer& client);
	
	/** Generates new chunks. */
	sptr<ServerGenStream> genStream = nullptr;

	/** The seed for generating the world. */
	u32 seed;
	
	/** A reference to the client list. */
	ServerClients& clients;
	
	/** The server inventories. */
	sptr<ServerInventoryRefs> refs;

//    std::string worldDir;
//    std::shared_ptr<FileManipulator> fileManip;
	
	usize totalGens = 0;

	/** The amount of mapblocks that were generated last frame. */
	u32 generatedMapBlocks = 0;
	
	/** A vector of positions for the order to generate mapblocks in. */
	vec<ivec3> generateOrder;

	/** The range in mapblocks to generate around clients. */
	const ivec2 mapBlockGenRange = { 6, 6 };
	
	/** The range in mapblocks to send to clients. */
	const ivec2 sendRange = { 6, 6 };

	/** The range around clients that chunks should be updated. */
	const ivec2 activeChunkRange = { 4, 4 };
};
