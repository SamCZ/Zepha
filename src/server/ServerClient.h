//
// Created by aurailus on 2020-07-30.
//

#pragma once

#include <enet/enet.h>

#include "util/Types.h"

class ServerPlayer;

class ServerClient {
	public:
	ServerClient(const string& username, ENetPeer* peer) :
		username(username), peer(peer), address(peer->address), id(peer->connectID) {}
	
	u32 id = 0;
	string username;
	
	ENetPeer* peer;
	ENetAddress address;
	
	std::shared_ptr<ServerPlayer> player = nullptr;
};
