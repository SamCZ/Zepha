//
// Created by aurailus on 11/01/19.
//

#pragma once

#include <memory>
#include <functional>
#include <glm/vec3.hpp>

class Model;
class Player;
class LocalWorld;
class LocalSubgame;
class PacketView;
class ServerConnection;

class ClientNetworkInterpreter {
public:
    ClientNetworkInterpreter(ServerConnection& connection, LocalSubgame& defs, Player& player);
    ClientNetworkInterpreter(const ClientNetworkInterpreter& other) = default;

    void init(LocalWorld* world, std::function<void(std::unique_ptr<PacketView>)> invCallback);
    void update();

    void blockPlace(glm::ivec3 pos, unsigned int block);
    void blockInteract(glm::ivec3 pos);

    void invWatch(const std::string& inv, const std::string& list);
    void invUnwatch(const std::string& inv, const std::string& list);
    void invInteractPrimary(const std::string& inv, const std::string& list, unsigned short ind);
    void invInteractSecondary(const std::string& inv, const std::string& list, unsigned short ind);

    int recvPackets = 0;
    int serverSideChunkGens = 0;
private:
    void receivedPacket(std::unique_ptr<PacketView> ePacket);

    Player& player;
    ServerConnection& connection;

    LocalWorld* world = nullptr;
    std::shared_ptr<Model> playerModel;

    std::function<void(std::unique_ptr<PacketView>)> onInvPacket;
};

