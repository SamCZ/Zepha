#pragma once

#include <enet/enet.h>

#include "util/Types.h"

class ServerPlayer;

/**
 * A connected client, containing a reference
 * to the raw ENet data it was initialized with.
 * The ENetPeer is given a pointer to an instance of this struct on connection.
 */

class ServerClient {
public:
	
	ServerClient(const string& username, u32 id, ENetPeer* peer) :
		username(username), peer(peer), address(peer->address), id(id) {}
	
	u32 id = 0;
	string username;
	
	ENetPeer* peer;
	ENetAddress address;
	
	sptr<ServerPlayer> player = nullptr;
};
