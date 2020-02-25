//
// Created by aurailus on 11/01/19.
//

#pragma once

#include "ServerConnection.h"
#include "../world/LocalWorld.h"

class ClientNetworkInterpreter {
public:
    ClientNetworkInterpreter(ServerConnection& connection, ClientGame& defs, Player& player);
    ClientNetworkInterpreter(const ClientNetworkInterpreter& other) = default;

    void init(LocalWorld* world);
    void update();

    void receivedPacket(std::unique_ptr<Packet> ePacket);

    void setBlock(glm::ivec3 pos, unsigned int block);
    void watchInv(const std::string& inv, const std::string& list);
    void unwatchInv(const std::string& inv, const std::string& list);

    ~ClientNetworkInterpreter();

    int recvPackets = 0;
    int serverSideChunkGens = 0;
    std::vector<std::unique_ptr<Packet>> chunkPackets;
private:
    Player& player;
    ServerConnection& connection;

    LocalWorld* world = nullptr;
    std::shared_ptr<Model> playerModel;

    int id = 0;
};

