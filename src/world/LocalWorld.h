#pragma once

#include "World.h"

#include "client/Window.h"
#include "client/gui/DebugGui.h"
#include "world/dim/LocalDimension.h"
#include "client/conn/ClientNetworkInterpreter.h"

class Renderer;
class PerfTimer;
class LocalPlayer;
class LocalSubgame;
class LocalInventoryRefs;
class WorldInterpolationStream;

/**
 * Manages the local active dimension,
 * and communication between the client and the server.
 */

class LocalWorld : public World {
public:
	
	LocalWorld(SubgamePtr game, ServerConnection& conn, Renderer& window, vec<string>& perfSections);
	
	/** Initializes the world, binding callbacks to the network handler. */
	void init();
	
	/** Sets the player's dimension to the default one, if it exists. */
	bool updatePlayerDimension();
	
	/** Updates the dimension and the player, reads incoming packets, and commits decoded chunks. */
	void update(f64 delta, vec<usize>& perfTimings, PerfTimer& perf);
	
	/** Queues a packet to be read. */
	void handleWorldPacket(uptr<PacketView> p);
	
	/** Reads a player entity packet, and updates the required entity. */
	void handlePlayerEntPacket(uptr<PacketView> p);
	
	/** Triggers a callback in lua for the provided mod message. */
	void handleModMessage(const string& channel, const string& message);
	
	/** Sets a chunk in the current dimension. */
	void commitChunk(sptr<Chunk> chunk);
	
	/** Sends a mod message to the server on the channel specified. */
	virtual void sendMessage(const string& channel, const string& message) override;
	
	/** Creates a new dimension with the identifier and biomes provided. */
	virtual DimensionPtr createDimension(const string& identifier, std::unordered_set<string>& biomes) override;
	
	/** Gets the active dimension. */
	DimensionPtr getActiveDimension();
	
	/** Sets the active dimension. */
	void setActiveDimension(DimensionPtr);
	
	/** Gets the local player. */
	PlayerPtr getPlayer();
	
	/** Returns a reference to the local inventory refs. */
	virtual InventoryRefsPtr getRefs() override;
	
	/** Returns a reference to the network handler. */
	ClientNetworkInterpreter& getNet();
	
	/** Renders the visible chunks to the screen. */
	void drawChunks();
	
	/** Renders the entities to the screen. */
	void drawEntities();
	
	/** Renders the interface to the screen using an orthographic projection. */
	void drawInterface();
	
private:

	/** A reference to the renderer. */
	Renderer& renderer;
	
	/** The network handler. */
	ClientNetworkInterpreter net;
	
	/** The local inventories. */
	sptr<LocalInventoryRefs> refs;
	
	/** The local player. */
	PlayerPtr player;
	
	/** The debug interface. */
	DebugGui debugGui;
	
	/** The number of chunks that were interpolated last frome. */
	u32 lastInterpolations = 0;
	
	/** Whether or not the hud should be visible. */
	bool hudVisible = true;
	
	/** Whether or not the debug interface should be visible. */
	bool debugVisible = true;
	
	/** A pointer to the active dimension. */
	sptr<LocalDimension> activeDimension = nullptr;
	
	Window::RCBLock lock;
	
	/** A reference to the world gen stream. */
//	sptr<WorldInterpolationStream> worldGenStream;
};
