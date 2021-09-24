#pragma once

#include <memory>

#include "Subgame.h"

#include "util/Types.h"
#include "lua/ServerLuaParser.h"
#include "game/atlas/ServerBiomeAtlas.h"
#include "game/atlas/asset/AssetStorage.h"
#include "game/atlas/ServerDefinitionAtlas.h"

class ServerWorld;
class ServerClients;

/**
 * Stores all subgame data for a server instance, including
 * block & item definitions, the lua sandbox, assets, and textures.
 */
 
class ServerSubgame : public Subgame {
public:
	
	ServerSubgame(const string& subgame, usize seed);
	
	~ServerSubgame() {};
	
	void init(WorldPtr world);
	
	void update(f64 delta);
	
	string subgamePath;
	
	ServerDefinitionAtlas& getDefs() override { return *defs; };
	
	ServerBiomeAtlas& getBiomes() override { return *biomes; };
	
	ServerLuaParser& getParser() override { return *lua; };
	
	AssetStorage assets;
	
private:
	
	uptr<ServerLuaParser> lua;
	uptr<ServerBiomeAtlas> biomes;
	uptr<ServerDefinitionAtlas> defs;
};

