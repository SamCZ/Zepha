//
// Created by aurailus on 11/01/19.
//

#pragma once

#include <memory>
#include <functional>
#include <glm/vec3.hpp>

#include "../Serializer.h"
#include "ServerConnection.h"

class Model;
class LocalPlayer;
class Target;
class LocalWorld;
class PacketView;
class LocalSubgame;
enum class NetPlayerField;

class ClientNetworkInterpreter {
public:
    ClientNetworkInterpreter(const ClientNetworkInterpreter& other) = delete;
    ClientNetworkInterpreter(ServerConnection& connection, LocalSubgame& defs, LocalWorld& world);

    void init(std::function<void(std::unique_ptr<PacketView>)> invCallback);
    void update();

    void blockPlace(Target& target);
    void blockInteract(Target& target);
    void blockPlaceOrInteract(Target& target);

    void invWatch(const std::string& inv, const std::string& list);
    void invUnwatch(const std::string& inv, const std::string& list);
    void invInteract(const std::string& inv, const std::string& list, bool primary, unsigned short ind);

    void sendPacket(const Packet& packet, PacketChannel channel);

    int recvPackets = 0;
    int serverSideChunkGens = 0;
private:
    void receivedPacket(std::unique_ptr<PacketView> ePacket);

    LocalWorld& world;
    ServerConnection& connection;

    std::function<void(std::unique_ptr<PacketView>)> onInvPacket;
};

