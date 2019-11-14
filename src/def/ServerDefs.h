//
// Created by aurailus on 10/06/19.
//

#pragma once


#include "ServerDefinitionAtlas.h"
#include "../lua/server/ServerLuaParser.h"
#include "../server/asset/AssetStorage.h"
#include "gen/BiomeStore.h"

class ServerWorld;

class ServerDefs {
public:
    void init(ServerWorld& world, const std::string& path);

    ServerDefinitionAtlas& defs();
    ServerLuaParser& lua();
    BiomeStore& gen();
    
    AssetStorage& assets();
    void update(double delta);

    ~ServerDefs() = default;
private:
    double delta = 0;

    AssetStorage assetStorage;
    ServerDefinitionAtlas blockAtlas;
    ServerLuaParser luaApi;
    BiomeStore biomes;
};

