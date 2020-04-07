//
// Created by aurailus on 09/01/19.
//

#pragma once

#include "conn/ClientList.h"
#include "world/ServerWorld.h"
#include "config/ServerConfig.h"
#include "../def/ServerGame.h"
#include "../util/net/NetHandler.h"
#include "../game/inventory/InventoryRefs.h"

class ServerClient;
class Packet;

class Server {
public:
    explicit Server(unsigned short port, const std::string& subgame);

    void update();
    void handlePlayerPacket(ServerClient& client, PacketView& p);

    void cleanup();

    ~Server();
private:
    bool alive = true;

    unsigned int seed = 0;
    unsigned short port = 0;

    ServerGame defs;
    ServerWorld world;
    NetHandler handler;
    ClientList clientList;
    ServerConfig config;
    InventoryRefs refs;

    std::unordered_set<unsigned int> playersUpdated {};

    double elapsedSeconds = 0;
    double deltaTime = 0;
};

