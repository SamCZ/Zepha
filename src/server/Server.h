//
// Created by aurailus on 09/01/19.
//

#pragma once

#include "world/ServerWorld.h"
#include "game/ServerSubgame.h"
#include "util/net/NetHandler.h"
#include "server/ServerConfig.h"
#include "server/ServerClients.h"
#include "world/inv/ServerInventoryRefs.h"

class ServerPlayer;
class Packet;

class Server {
public:
    explicit Server(unsigned short port, const std::string& subgame);

    void update();

    void cleanup();
    ~Server();
private:
    void packetReceived(ENetEvent& e);
    void playerPacketReceived(PacketView& p, PlayerPtr player);

    unsigned short port = 0;
    unsigned int seed = 0;

    SubgamePtr game;
    ServerClients clients;
    WorldPtr world;
    NetHandler handler;
    ServerConfig config;

    std::unordered_set<unsigned int> playersUpdated {};

    double elapsed = 0;
    double delta = 0;

    bool alive = true;
};

