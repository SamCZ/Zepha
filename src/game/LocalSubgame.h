/*
 * The Local Subgame stores all local subgame data.
 * Includes Block / Item definitions, biome definitions, the lua parser, models, and textures.
 *
 * - Auri, 03/11/20
 */

#pragma once

#include "Subgame.h"

#include "util/CovariantPtr.h"
#include "lua/LocalLuaParser.h"
#include "game/atlas/TextureAtlas.h"
#include "game/atlas/LocalBiomeAtlas.h"
#include "game/atlas/asset/ModelStore.h"
#include "game/atlas/LocalDefinitionAtlas.h"

class Client;

class LocalWorld;

class LocalPlayer;

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
	std::shared_ptr<LocalLuaParser> lua;
	std::shared_ptr<LocalBiomeAtlas> biomes;
	std::shared_ptr<LocalDefinitionAtlas> defs;
};

