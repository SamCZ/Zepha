//
// Created by aurailus on 18/04/19.
//

#include "LocalSubgame.h"


/**
 * Creates subgame instances, including the lua parser, and biome and block atlases.
 * Also loads base assets into the texture atlas.
 * @param baseAssets - The relative path to the base texture assets.
 */

LocalSubgame::LocalSubgame(const std::string& baseAssets) :
	textures(2048),
	
	lua(std::make_unique<LocalLuaParser>(*this)),
	biomes(std::make_unique<LocalBiomeAtlas>()),
	defs(std::make_unique<LocalDefinitionAtlas>(textures)) {
	
	textures.loadDirectory(baseAssets);
}


/**
 * Initializes the Lua Parser.
 * @param world - Passed in to the Lua Parser.
 * @param player - Passed in to the Lua Parser.
 * @param client - Passed in to the Lua Parser.
 */

void LocalSubgame::init(WorldPtr world, PlayerPtr player, Client& client) {
	lua->init(world, player, client);
}


/**
 * Updates the lua parser and the texture atlas.
 * @param delta - The last frame's delta time.
 */

void LocalSubgame::update(double delta) {
	lua->update(delta);
	textures.update();
}
