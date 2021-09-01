#pragma once

#include <enet.h>
#include <unordered_map>

#include "util/Types.h"
#include "util/CovariantPtr.h"

class ServerWorld;
class ServerClient;
class ServerPlayer;
class ServerSubgame;
class ServerDimension;

/**
 * Manages creating and deleting clients on connection
 * and disconnection, and handles creating client players.
 */

class ServerClients {
public:
	
	explicit ServerClients(SubgamePtr game);
	
	void init(ServerWorld* world);
	
	/** Creates a new client from the event specified. */
	void handleConnect(ENetEvent e);

	/** Removes a client from the event specified. */
	void handleDisconnect(ENetEvent e);
	
	/** Creates a player for the client specified. */
	void createPlayer(sptr<ServerClient> client, DimensionPtr dimension);

	/** Returns the server client for the id specified. */
	const sptr<ServerClient> getClient(u32 id) const;
	
	/** Returns the server clients map. */
	const std::unordered_map<u32, sptr<ServerClient>>& getClients() const;
	
private:
	
	SubgamePtr game;
	ServerWorld* world = nullptr;
	
	/** The internal clients map. */
	std::unordered_map<u32, sptr<ServerClient>> clients;
};
