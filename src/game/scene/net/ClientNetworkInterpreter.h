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

    void init(LocalWorld* world, std::function<void(std::unique_ptr<PacketView>)> invCallback);
    void update();

    // Functions to be called by outside of ClientNetworkInterpreter
    void setBlock(glm::ivec3 pos, unsigned int block);
    void watchInv(const std::string& inv, const std::string& list);
    void unwatchInv(const std::string& inv, const std::string& list);

    void primaryInteract(const std::string& inv, const std::string& list, unsigned short ind);
    void secondaryInteract(const std::string& inv, const std::string& list, unsigned short ind);

    int recvPackets = 0;
    int serverSideChunkGens = 0;
private:
    void receivedPacket(std::unique_ptr<PacketView> ePacket);

    int cid = 0;
    Player& player;
    ServerConnection& connection;

    LocalWorld* world = nullptr;
    std::shared_ptr<Model> playerModel;

    std::function<void(std::unique_ptr<PacketView>)> onInvPacket;
};

