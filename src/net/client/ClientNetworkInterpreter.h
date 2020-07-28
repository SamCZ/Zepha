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
class Player;
class Target;
class LocalWorld;
class PacketView;
class LocalSubgame;
enum class NetPlayerField;

class ClientNetworkInterpreter {
public:
    ClientNetworkInterpreter(ServerConnection& connection, LocalSubgame& defs, Player& player);
    ClientNetworkInterpreter(const ClientNetworkInterpreter& other) = default;

    void init(LocalWorld* world, std::function<void(std::unique_ptr<PacketView>)> invCallback);
    void update();

    void blockPlace(Target& target);
    void blockInteract(Target& target);
    void blockPlaceOrInteract(Target& target);

    void invWatch(const std::string& inv, const std::string& list);
    void invUnwatch(const std::string& inv, const std::string& list);
    void invInteractPrimary(const std::string& inv, const std::string& list, unsigned short ind);
    void invInteractSecondary(const std::string& inv, const std::string& list, unsigned short ind);

    template <typename T> void assertPlayerField(NetPlayerField field, T data) {
        Serializer().append(static_cast<unsigned int>(field)).append<T>(data)
            .packet(PacketType::THIS_PLAYER_INFO).sendTo(connection.getPeer(), PacketChannel::INTERACT);
    }

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

