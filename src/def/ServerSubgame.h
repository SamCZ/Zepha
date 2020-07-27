//
// Created by aurailus on 10/06/19.
//

#pragma once

#include <memory>

#include "Subgame.h"

#include "../def/gen/ServerBiomeAtlas.h"
#include "../def/ServerDefinitionAtlas.h"
#include "../lua/ServerLuaParser.h"
#include "../net/server/asset/AssetStorage.h"

class ServerWorld;
class ClientList;

class ServerSubgame : public Subgame {
public:
    ServerSubgame(const std::string& subgame, unsigned int seed);
    ~ServerSubgame();

    void init(ServerWorld& world);
    void update(double delta, ClientList& clients);

    std::string subgamePath;

    ServerDefinitionAtlas& getDefs() override { return *defs; };
    ServerBiomeAtlas& getBiomes() override { return *biomes; };
    ServerLuaParser& getParser() override { return *lua; };

//private:
    AssetStorage assets;

    std::unique_ptr<ServerLuaParser> lua;
    std::unique_ptr<ServerBiomeAtlas> biomes;
    std::unique_ptr<ServerDefinitionAtlas> defs;
};

