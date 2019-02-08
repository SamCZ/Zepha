//
// Created by aurailus on 11/01/19.
//

#include "ServerConnection.h"

ServerConnection::ServerConnection(std::string address, unsigned short port) {
    this->port = port;
    this->address = std::move(address);
}

void ServerConnection::init() {
    handler = NetHandler(address, port, 3, 3000);

    if (handler.getState() != NetHandler::CLIENT) {
        exit(EXIT_FAILURE);
    }
}


void ServerConnection::update(Player &player, Entity& pointer) {
    ENetEvent event;
    while (handler.update(&event)) {

        switch (event.type) {
            case ENET_EVENT_TYPE_CONNECT: {
                printf("[INFO] Connected to server %x:%u.\n", event.peer->address.host, event.peer->address.port);
                break;
            }
            case ENET_EVENT_TYPE_RECEIVE: {
                Packet p(event.packet);

                if (p.type == PacketType::PLAYERINFO) {
                    glm::vec3 playerPos = glm::vec3(
                            Serializer::decodeFloat(&p.data[0]),
                            Serializer::decodeFloat(&p.data[4]),
                            Serializer::decodeFloat(&p.data[8])
                    );
                    player.setPos(playerPos);
                }
                else if (p.type == PacketType::ENTITYINFO) {
                    glm::vec3 playerPos = glm::vec3(
                            Serializer::decodeFloat(&p.data[0]),
                            Serializer::decodeFloat(&p.data[4]),
                            Serializer::decodeFloat(&p.data[8])
                    );
                    pointer.setPosition(playerPos);
                }

                enet_packet_destroy(event.packet);
                break;
            }
            case ENET_EVENT_TYPE_DISCONNECT: {
                printf("[INFO] Disconnected from server %x:%u\n", event.peer->address.host, event.peer->address.port);
                break;
            }

            default:
                break;
        }
    }

    //Send Player Position
    Packet p(PacketType::PLAYERINFO);
    Serializer::encodeFloat(p.data, player.getPos()->x);
    Serializer::encodeFloat(p.data, player.getPos()->y);
    Serializer::encodeFloat(p.data, player.getPos()->z);
    p.sendTo(handler.getPeer(), 0);
}

void ServerConnection::cleanup() {
    connected = false;
}

ServerConnection::~ServerConnection() {
    cleanup();
}