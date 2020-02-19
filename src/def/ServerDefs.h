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

class ServerDefs {
public:
    ServerDefs(const std::string& subgame, const std::string& execPath);
    void init(ServerWorld& world);

    void update(double delta, ClientList& clients);

    ~ServerDefs() = default;
    std::string subgamePath = "";

    ServerDefinitionAtlas defs;
    ServerLuaParser luaApi;
    ServerBiomeAtlas biomes;
    AssetStorage assets;
};

