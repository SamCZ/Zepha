//
// Created by aurailus on 10/06/19.
//

#pragma once

#include "Game.h"

#include "../net/server/asset/AssetStorage.h"

class ServerWorld;
class ClientList;

class ServerLuaParser;
class ServerBiomeAtlas;
class ServerDefinitionAtlas;

class ServerGame : public Game {
public:
    ServerGame(const std::string& subgame, unsigned int seed);
    ~ServerGame();

    void init(ServerWorld& world);
    void update(double delta, ClientList& clients);

    std::string subgamePath;

    AssetStorage assets;

    std::unique_ptr<ServerLuaParser> lua;
    std::unique_ptr<ServerBiomeAtlas> biomes;
    std::unique_ptr<ServerDefinitionAtlas> defs;
};

