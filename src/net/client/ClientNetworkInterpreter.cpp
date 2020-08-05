//
// Created by aurailus on 11/01/19.
//

#include <iostream>

#include "ClientNetworkInterpreter.h"

#include "../PacketView.h"
#include "../Serializer.h"
#include "../NetHandler.h"
#include "../../util/Log.h"
#include "../../game/scene/world/NetField.h"
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
        .appendE(NetField::POS).append(player->getPos())
        .appendE(NetField::VEL).append(player->getVel())
        .appendE(NetField::LOOK_PITCH).append(player->getPitch())
        .appendE(NetField::LOOK_YAW).append(player->getYaw())
        .packet(Packet::Type::THIS_PLAYER_INFO, false).sendTo(connection.getPeer(), Packet::Channel::INTERACT);
}

void ClientNetworkInterpreter::receivedPacket(std::unique_ptr<PacketView> p) {
    switch (p->type) {
        default:
            std::cout << Log::err << "Received unknown packet of type " << static_cast<int>(p->type)
                << ". Is the server on a different protocol version?" << Log::endl; break;

        case Packet::Type::SERVER_INFO:
            serverSideChunkGens = p->d.read<unsigned int>(); break;

        case Packet::Type::THIS_PLAYER_INFO:
            world.getPlayer()->handleAssertion(p->d); break;
            
        case Packet::Type::PLAYER_ENT_INFO:
            world.handlePlayerEntPacket(std::move(p)); break;

        case Packet::Type::CHUNK:
        case Packet::Type::MAPBLOCK:
            world.handleWorldPacket(std::move(p)); break;
            
        case Packet::Type::BLOCK_SET: {
            auto pos = p->d.read<glm::ivec3>();
            auto block = p->d.read<unsigned int>();
            world.getActiveDimension()->setBlock(pos, block);
            break; }

        case Packet::Type::ENTITY_INFO:
            world.getActiveDimension().l()->serverEntitiesInfo(p->d); break;

        case Packet::Type::ENTITY_REMOVED:
            world.getActiveDimension().l()->serverEntitiesRemoved(p->d); break;

        case Packet::Type::INV_DATA:
            onInvPacket(std::move(p)); break;
        
        case Packet::Type::INV_INVALID: {
            std::string source = p->d.read<std::string>();
            std::string list = p->d.read<std::string>();
            throw std::runtime_error("Invalid inventory " + source + ":" + list + " was request by client.");
            break; }
    }
}

void ClientNetworkInterpreter::blockHit(const Target &target) {
    Serializer().append<glm::ivec3>(target.pos).append(static_cast<unsigned short>(target.face))
            .packet(Packet::Type::BLOCK_HIT).sendTo(connection.getPeer(), Packet::Channel::INTERACT);
}

void ClientNetworkInterpreter::blockPlace(const Target &target) {
    Serializer().append<glm::ivec3>(target.pos).append(static_cast<unsigned short>(target.face))
        .packet(Packet::Type::BLOCK_PLACE).sendTo(connection.getPeer(), Packet::Channel::INTERACT);
}

void ClientNetworkInterpreter::blockInteract(const Target &target) {
    Serializer().append<glm::ivec3>(target.pos).append(static_cast<unsigned short>(target.face))
        .packet(Packet::Type::BLOCK_INTERACT).sendTo(connection.getPeer(), Packet::Channel::INTERACT);
}

void ClientNetworkInterpreter::blockPlaceOrInteract(const Target &target) {
    Serializer().append<glm::ivec3>(target.pos).append(static_cast<unsigned short>(target.face))
        .packet(Packet::Type::BLOCK_PLACE_OR_INTERACT).sendTo(connection.getPeer(), Packet::Channel::INTERACT);
}

void ClientNetworkInterpreter::invWatch(const std::string& inv, const std::string& list) {
    Serializer().append(inv).append(list).packet(Packet::Type::INV_WATCH)
        .sendTo(connection.getPeer(), Packet::Channel::INTERACT);
}

void ClientNetworkInterpreter::invUnwatch(const std::string& inv, const std::string& list) {
    Serializer().append(inv).append(list).packet(Packet::Type::INV_UNWATCH)
        .sendTo(connection.getPeer(), Packet::Channel::INTERACT);
}

void ClientNetworkInterpreter::invInteract(const std::string &inv, const std::string &list, bool primary, unsigned short ind) {
    Serializer().append<unsigned short>(primary).append(inv).append(list).append<unsigned short>(ind)
        .packet(Packet::Type::INV_INTERACT).sendTo(connection.getPeer(), Packet::Channel::INTERACT);
}

void ClientNetworkInterpreter::sendPacket(const Packet &packet, Packet::Channel channel) {
    packet.sendTo(connection.getPeer(), channel);
}
