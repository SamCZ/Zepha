//
// Created by aurailus on 09/01/19.
//

#pragma once

#include "util/Types.h"
#include "world/ServerWorld.h"
#include "game/ServerSubgame.h"
#include "util/net/NetHandler.h"
#include "server/ServerConfig.h"
#include "server/ServerClients.h"
#include "world/inv/ServerInventoryRefs.h"

class ServerPlayer;

class Packet;

class Server {
public:
	explicit Server(u16 port, const std::string& subgame);
	
	void update();
	
	void cleanup();
	
	~Server();
	
	private:
	void packetReceived(ENetEvent& e);
	
	void playerPacketReceived(PacketView& p, PlayerPtr player);
	
	/** The server's port. */
	u16 port = 0;
	
	/** The server's seed. */
	u32 seed = 0;
	
	/** The subgame instance, must be listed before world. */
	SubgamePtr game;
	
	/** The world instance, needs to be after the subgame. */
	WorldPtr world;
	
	/** The network handler, manages client connections. */
	NetHandler handler;
	
	/** Handles passing data to newly connecting clients. TODO: Rename. */
	ServerConfig config;
	
	/** A list of connected clients. */
	ServerClients clients;
	
	/** A list of players who have been updated, to send to the other clients. */
	std::unordered_set<u32> playersUpdated{};
	
	/** The time elapsed since the start of the server. */
	f64 elapsed = 0;
	
	/** The delta time of the last frame. */
	f64 delta = 0;
	
	/** Whether or not the server should be kept running. */
	bool alive = true;
};

