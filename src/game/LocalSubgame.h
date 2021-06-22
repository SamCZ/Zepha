#pragma once

#include "Subgame.h"

#include "util/Types.h"
#include "util/CovariantPtr.h"
#include "lua/LocalLuaParser.h"
#include "game/atlas/TextureAtlas.h"
#include "game/atlas/LocalBiomeAtlas.h"
#include "game/atlas/asset/ModelStore.h"
#include "game/atlas/LocalDefinitionAtlas.h"

class Client;
class LocalWorld;
class LocalPlayer;

/**
 * Stores all subgame data for a local client, including
 * block & item definitions, the lua sandbox, models, and textures.
 */
 
class LocalSubgame : public Subgame {
public:
	
	explicit LocalSubgame(const std::string& baseAssets);
	
	void init(WorldPtr world, PlayerPtr player, Client& client);
	
	void update(double delta);
	
	LocalDefinitionAtlas& getDefs() override { return *defs; };
	
	LocalBiomeAtlas& getBiomes() override { return *biomes; };
	
	LocalLuaParser& getParser() override { return *lua; };
	
	ModelStore models;
	TextureAtlas textures;
	
private:
	
	uptr<LocalLuaParser> lua;
	uptr<LocalBiomeAtlas> biomes;
	uptr<LocalDefinitionAtlas> defs;
};

