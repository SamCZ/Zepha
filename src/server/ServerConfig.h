//
// Created by aurailus on 03/07/19.
//

#pragma once

#include "util/CovariantPtr.h"
#include "util/net/PacketView.h"

class PacketView;
class ServerClient;
class ServerSubgame;

class ServerConfig {
public:
	explicit ServerConfig(SubgamePtr game);
	
	void init();
	
	/**
	 * Handles a packet requesting subgame data.
	 * Returns a boolean indicating if the client is done receiving all data.
	 */
	 
	bool handlePacket(ServerClient& client, PacketView& p);
	
private:
	SubgamePtr game;
	
	vec<string> blockIdentifierList {};
	vec<string> biomeIdentifierList {};
};
