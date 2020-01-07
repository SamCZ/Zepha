//
// Created by aurailus on 09/01/19.
//

#pragma once

#include <thread>
#include <vector>
#include <iostream>

#include "conn/ServerPlayer.h"
#include "conn/ServerClient.h"
#include "conn/ServerClients.h"
#include "world/ServerWorld.h"
#include "config/ServerConfig.h"
#include "../def/ServerDefs.h"
#include "../util/net/Packet.h"
#include "../util/Timer.h"
#include "../util/net/NetHandler.h"
#include "../util/net/Serializer.h"

class Server {
public:
    explicit Server(const std::string& path, unsigned short port, const std::string& subgame);

    void update();
    void handlePlayerPacket(ServerClient& client, Packet& p);

    void cleanup();

    ~Server();
private:
    bool alive = true;

    ServerDefs defs;
    ServerWorld world;
    NetHandler handler;
    ServerClients clientList;
    ServerConfig config;

    double elapsedSeconds = 0;
    double deltaTime = 0;

    unsigned short port = 0;
};

