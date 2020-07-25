//
// Created by aurailus on 10/06/19.
//

#include <cute_files/cute_files.h>

#include "ServerGame.h"

#include "../def/gen/ServerBiomeAtlas.h"
#include "../def/ServerDefinitionAtlas.h"
#include "../net/server/conn/ClientList.h"
#include "../lua/parser/ServerLuaParser.h"

ServerGame::ServerGame(const std::string& subgame, unsigned int seed) :
    subgamePath("subgames/" + subgame + "/"),

    defs(std::make_unique<ServerDefinitionAtlas>()),
    biomes(std::make_unique<ServerBiomeAtlas>(seed)),
    lua(std::make_unique<ServerLuaParser>()) {

    if (subgame.empty()) throw std::runtime_error("No subgame specified.");
    else if (!cf_file_exists(subgamePath.data())) throw std::runtime_error("Subgame does not exist.");
}

void ServerGame::init(ServerWorld &world) {
    lua->init(*this, world, subgamePath);
}

void ServerGame::update(double delta, ClientList& clients) {
    lua->update(delta);
}

ServerGame::~ServerGame() {}
