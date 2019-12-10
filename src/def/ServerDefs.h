//
// Created by aurailus on 10/06/19.
//

#pragma once


#include "ServerDefinitionAtlas.h"
#include "../lua/server/ServerLuaParser.h"
#include "../server/asset/AssetStorage.h"
#include "gen/ServerBiomeAtlas.h"

class ServerWorld;

class ServerDefs {
public:
    ServerDefs(const std::string& subgame, const std::string& execPath);
    void init(ServerWorld& world);

    ServerDefinitionAtlas& defs();
    ServerLuaParser& lua();
    ServerBiomeAtlas& gen();
    
    AssetStorage& assets();
    void update(double delta);

    ~ServerDefs() = default;
private:
    double delta = 0;
    std::string subgamePath = "";

    AssetStorage assetStorage;
    ServerDefinitionAtlas blockAtlas;
    ServerLuaParser luaApi;
    ServerBiomeAtlas biomes;
};

