//
// Created by aurailus on 10/06/19.
//

#pragma once


#include "gen/ServerBiomeAtlas.h"
#include "ServerDefinitionAtlas.h"
#include "../server/asset/AssetStorage.h"
#include "../lua/parser/ServerLuaParser.h"

class ServerWorld;
class ClientList;

class ServerGame {
public:
    ServerGame(const std::string& subgame, unsigned int seed);
    void init(ServerWorld& world);
    void update(double delta, ClientList& clients);

    std::string subgamePath;

    ServerDefinitionAtlas defs;
    ServerLuaParser       parser;
    ServerBiomeAtlas      biomes;
    AssetStorage          assets;
};

