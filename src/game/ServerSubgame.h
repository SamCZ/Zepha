//
// Created by aurailus on 10/06/19.
//

#pragma once

#include <memory>

#include "Subgame.h"

#include "lua/ServerLuaParser.h"
#include "game/atlas/ServerBiomeAtlas.h"
#include "game/atlas/asset/AssetStorage.h"
#include "game/atlas/ServerDefinitionAtlas.h"

class ServerWorld;
class ServerClients;

class ServerSubgame : public Subgame {
public:
    ServerSubgame(const std::string& subgame, unsigned int seed);
    ~ServerSubgame();

    void init(WorldPtr world);
    void update(double delta);

    std::string subgamePath;

    ServerDefinitionAtlas& getDefs() override { return *defs; };
    ServerBiomeAtlas& getBiomes() override { return *biomes; };
    ServerLuaParser& getParser() override { return *lua; };

    AssetStorage assets;

private:

    std::unique_ptr<ServerLuaParser> lua;
    std::unique_ptr<ServerBiomeAtlas> biomes;
    std::unique_ptr<ServerDefinitionAtlas> defs;
};

