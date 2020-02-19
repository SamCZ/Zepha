//
// Created by aurailus on 11/01/19.
//

#include "../world/Player.h"
#include "../../../util/net/NetHandler.h"
#include "../../../util/net/Serializer.h"

#include "ClientNetworkInterpreter.h"

//#include <string>
//#include <iostream>
//#include <glm/vec3.hpp>
//
//#include "../../../util/Timer.h"
//#include "../../../util/net/Packet.h"
//#include "../../entity/world/PlayerEntity.h"
//#include "../../graph/drawable/DrawableGroup.h"
//#include "../../../util/net/Address.h"

ClientNetworkInterpreter::ClientNetworkInterpreter(ServerConnection &connection, LocalDefs &defs, Player& player) :
    world(nullptr),
    player(player),
    connection(connection),
    playerModel(std::make_shared<Model>()) {

    playerModel->fromSerialized(defs.models.models["zeus:default:player"], {defs.textures["zeus:default:player"]});
}

void ClientNetworkInterpreter::init(LocalWorld *world) {
    this->world = world;
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
                std::unique_ptr<Packet> p = std::make_unique<Packet>(event.packet);
                receivedPacket(std::move(p));
                enet_packet_destroy(event.packet);
                break;
            }
            case ENET_EVENT_TYPE_DISCONNECT: {
                std::cout << Log::info << "Disconnected from server "
                          << event.peer->address.host << ":" << event.peer->address.port << "." << Log::endl;
                break;
            }
        }
    }

    //Send Player Position
    Packet p(PacketType::THIS_PLAYER_INFO);
    p.data = Serializer()
            .append(player.getPos())
            .append(player.getPitch())
            .append(player.getYaw())
            .data;

    p.sendTo(connection.getPeer(), PacketChannel::PLAYER);
}

void ClientNetworkInterpreter::setBlock(glm::ivec3 pos, unsigned int block) {
    Packet p(PacketType::BLOCK_SET);
    Serializer()
        .append(pos)
        .append(block)
        .packet(PacketType::BLOCK_SET)
        .sendTo(connection.getPeer(), PacketChannel::BLOCK);
}

void ClientNetworkInterpreter::receivedPacket(std::unique_ptr<Packet> p) {
    Deserializer d(p->data);

    switch (p->type) {
        default: {
            std::cout << Log::err << "Received unknown packet of type " << static_cast<int>(p->type)
                      << ". Is the server on a different protocol version?" << Log::endl;
            break;
        }

        case PacketType::THIS_PLAYER_INFO: {
            id = d.read<unsigned int>();

            player.setPos(d.read<glm::vec3>());
            player.setYaw(d.read<float>());
            break;
        }

        case PacketType::PLAYER_INFO: {
            unsigned int cid = d.read<unsigned int>();
            if (cid == id) break;

            bool found = false;
            for (auto& entity : world->dimension.playerEntities) {
                if (entity.getCid() == cid) {
                    // Update an existing PlayerEntity
                    entity.interpPos(d.read<glm::vec3>());
                    entity.interpRotateY(-d.read<float>() + 90);
                    entity.interpRotateZ(-d.read<float>());

                    found = true;
                    break;
                }
            }
            if (found) break;

            // Instantiate a new PlayerEntity
            world->dimension.playerEntities.emplace_back(d.read<glm::vec3>(), cid, playerModel);
            break;
        }

        case PacketType::ENTITY_INFO: {
            world->dimension.handleServerEntity(*p);
            break;
        }

        case PacketType::BLOCK_SET: {
            auto pos = d.read<glm::ivec3>();
            auto block = d.read<unsigned int>();
            world->setBlock(pos, block);
            break;
        }

        case PacketType::CHUNK: {
            chunkPackets.push_back(std::move(p));
            break;
        }

        case PacketType::SERVER_INFO: {
            serverSideChunkGens = d.read<unsigned int>();
            break;
        }
    }
}

ClientNetworkInterpreter::~ClientNetworkInterpreter() {
    connection.disconnect();
}