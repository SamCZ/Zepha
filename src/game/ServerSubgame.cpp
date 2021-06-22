//
// Created by aurailus on 10/06/19.
//

#include <cute_files/cute_files.h>

#include "ServerSubgame.h"

#include "server/ServerClients.h"

ServerSubgame::ServerSubgame(const string& subgame, usize seed) :
	subgamePath("../subgames/" + subgame + "/"),
	defs(make_unique<ServerDefinitionAtlas>()),
	biomes(make_unique<ServerBiomeAtlas>(seed)),
	lua(make_unique<ServerLuaParser>(*this)) {
	
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
