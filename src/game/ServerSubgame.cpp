//
// Created by aurailus on 10/06/19.
//

#include <cute_files/cute_files.h>

#include "ServerSubgame.h"

#include "server/ServerClients.h"

ServerSubgame::ServerSubgame(const std::string& subgame, unsigned int seed) :
	subgamePath("../subgames/" + subgame + "/"),
	
	defs(std::make_unique<ServerDefinitionAtlas>()),
	biomes(std::make_unique<ServerBiomeAtlas>(seed)),
	lua(std::make_unique<ServerLuaParser>(*this)) {
	
	if (subgame.empty()) throw std::runtime_error("No subgame specified.");
	else if (!cf_file_exists(subgamePath.data())) throw std::runtime_error("Subgame does not exist.");
}

void ServerSubgame::init(WorldPtr world) {
	lua->init(world, subgamePath);
}

void ServerSubgame::update(double delta) {
	lua->update(delta);
}

ServerSubgame::~ServerSubgame() {}
