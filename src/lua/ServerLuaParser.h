//
// Created by aurailus on 11/06/19.
//

#pragma once

#include <enet.h>
#include "LuaParser.h"
#include "ServerModHandler.h"

class ServerWorld;
class ServerPlayer;
class ServerSubgame;

class ServerLuaParser : public LuaParser {
public:
	explicit ServerLuaParser(ServerSubgame& game);
	
	void init(WorldPtr world, const std::string& rootPath);
	
	virtual void update(double delta) override;
	
	void sendModsPacket(ENetPeer* peer) const;
	
	void playerConnected(std::shared_ptr<ServerPlayer> client);
	
	void playerDisconnected(std::shared_ptr<ServerPlayer> client);
	
private:
	void loadApi(WorldPtr world);
	
	/** Exposed to Lua, can provide a relative path, uses environment variables to resolve. */
	sol::protected_function_result require(sol::this_environment env, string requirePath);
	
	/** Loads a file with the right enviroment, needs a canonical path. */
	sol::protected_function_result loadFile(string path);
	
	ServerModHandler handler;
	double accumulatedDelta = 0;
};

