//
// Created by aurailus on 11/01/19.
//

#include <iostream>

#include "ClientNetworkInterpreter.h"

#include "../PacketView.h"
#include "../Serializer.h"
#include "../NetHandler.h"
#include "../../util/Log.h"
#include "../../game/entity/Model.h"
#include "../../game/scene/world/LocalWorld.h"
#include "../../game/scene/world/LocalPlayer.h"

ClientNetworkInterpreter::ClientNetworkInterpreter(ServerConnection &connection, LocalWorld& world) :
    world(world), connection(connection) {}

void ClientNetworkInterpreter::init(std::function<void(std::unique_ptr<PacketView>)> invCallback) {
    this->onInvPacket = invCallback;
}

void ClientNetworkInterpreter::update() {
    recvPackets = 0;

    ENetEvent event;
    while (connection.pollEvents(&event)) {
        recvPackets++;

        switch (event.type) {
            default: break;
            case ENET_EVENT_TYPE_CONNECT: {
                std::cout << Log::info << "Connected to server "
                          << NetHandler::intToIPString(event.peer->address.host)
                          << ":" << event.peer->address.port << "." << Log::endl;
                break;
            }
            case ENET_EVENT_TYPE_RECEIVE: {
                std::unique_ptr<PacketView> p = std::make_unique<PacketView>(event.packet);
                receivedPacket(std::move(p));
                break;
            }
            case ENET_EVENT_TYPE_DISCONNECT: {
                std::cout << Log::info << "Disconnected from server "
                          << event.peer->address.host << ":" << event.peer->address.port << "." << Log::endl;
                break;
            }
        }
    }

    auto player = world.getPlayer();
    if (player) Serializer()
        .appendE(Player::NetField::POS).append(player->getPos())
        .appendE(Player::NetField::VEL).append(player->getVel())
        .appendE(Player::NetField::PITCH).append(player->getPitch())
        .appendE(Player::NetField::YAW).append(player->getYaw())
        .packet(PacketType::THIS_PLAYER_INFO, false).sendTo(connection.getPeer(), PacketChannel::INTERACT);
}

void ClientNetworkInterpreter::receivedPacket(std::unique_ptr<PacketView> p) {
    switch (p->type) {
        default:
            std::cout << Log::err << "Received unknown packet of type " << static_cast<int>(p->type)
                << ". Is the server on a different protocol version?" << Log::endl; break;

        case PacketType::SERVER_INFO:
            serverSideChunkGens = p->d.read<unsigned int>(); break;

        case PacketType::THIS_PLAYER_INFO:
            if (!world.getPlayer()) throw std::runtime_error("Received player info *before* the player was created.");
            world.getPlayer()->handleAssertion(p->d); break;
            
        case PacketType::PLAYER_ENT_INFO:
            world.handlePlayerEntPacket(std::move(p)); break;

        case PacketType::CHUNK:
        case PacketType::MAPBLOCK:
            world.handleWorldPacket(std::move(p)); break;
            
        case PacketType::BLOCK_SET: {
            auto pos = p->d.read<glm::ivec3>();
            auto block = p->d.read<unsigned int>();
            world.getActiveDimension()->setBlock(pos, block);
            break; }

        case PacketType::ENTITY_INFO:
            world.getActiveDimension().l()->serverEntityInfo(*p); break;

        case PacketType::ENTITY_REMOVED:
            world.getActiveDimension().l()->serverEntityRemoved(p->d.read<unsigned int>()); break;

        case PacketType::INV_DATA:
            onInvPacket(std::move(p)); break;
        
        case PacketType::INV_INVALID: {
            std::string source = p->d.read<std::string>();
            std::string list = p->d.read<std::string>();
            throw std::runtime_error("Invalid inventory " + source + ":" + list + " was request by client.");
            break; }
    }
}

void ClientNetworkInterpreter::blockPlace(Target &target) {
    Serializer().append<glm::ivec3>(target.pos).append(static_cast<unsigned short>(target.face))
        .packet(PacketType::BLOCK_PLACE).sendTo(connection.getPeer(), PacketChannel::INTERACT);
}

void ClientNetworkInterpreter::blockInteract(Target &target) {
    Serializer().append<glm::ivec3>(target.pos).append(static_cast<unsigned short>(target.face))
        .packet(PacketType::BLOCK_INTERACT).sendTo(connection.getPeer(), PacketChannel::INTERACT);
}

void ClientNetworkInterpreter::blockPlaceOrInteract(Target &target) {
    Serializer().append<glm::ivec3>(target.pos).append(static_cast<unsigned short>(target.face))
        .packet(PacketType::BLOCK_PLACE_OR_INTERACT).sendTo(connection.getPeer(), PacketChannel::INTERACT);
}

void ClientNetworkInterpreter::invWatch(const std::string& inv, const std::string& list) {
    Serializer().append(inv).append(list).packet(PacketType::INV_WATCH)
        .sendTo(connection.getPeer(), PacketChannel::INVENTORY);
}

void ClientNetworkInterpreter::invUnwatch(const std::string& inv, const std::string& list) {
    Serializer().append(inv).append(list).packet(PacketType::INV_UNWATCH)
        .sendTo(connection.getPeer(), PacketChannel::INVENTORY);
}

void ClientNetworkInterpreter::invInteract(const std::string &inv, const std::string &list, bool primary, unsigned short ind) {
    Serializer().append<unsigned short>(primary).append(inv).append(list).append<unsigned short>(ind)
        .packet(PacketType::INV_INTERACT).sendTo(connection.getPeer(), PacketChannel::INVENTORY);
}

void ClientNetworkInterpreter::sendPacket(const Packet &packet, PacketChannel channel) {
    packet.sendTo(connection.getPeer(), channel);
}
