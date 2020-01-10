//
// Created by aurailus on 10/06/19.
//

#pragma once


#include "ServerDefinitionAtlas.h"
#include "../lua/client/ServerLuaParser.h"
#include "../server/asset/AssetStorage.h"
#include "gen/ServerBiomeAtlas.h"

class ServerWorld;

class ServerDefs {
public:
    ServerDefs(const std::string& subgame, const std::string& execPath);
    void init(ServerWorld& world);

    void update(double delta);

    ~ServerDefs() = default;
    std::string subgamePath = "";

    ServerDefinitionAtlas defs;
    ServerLuaParser luaApi;
    ServerBiomeAtlas biomes;
    AssetStorage assets;
};

