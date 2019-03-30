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


void ServerConnection::update(Player &player, std::vector<PlayerEntity*>& playerEntities) {
    recvPackets = 0;

    ENetEvent event;
    while (handler.update(&event)) {
        recvPackets++;

        switch (event.type) {
            case ENET_EVENT_TYPE_CONNECT: {
                printf("[INFO] Connected to server %x:%u.\n", event.peer->address.host, event.peer->address.port);
                break;
            }
            case ENET_EVENT_TYPE_RECEIVE: {
                Packet* p = new Packet(event.packet);

                switch (p->type) {
                    case Packet::PLAYER_INFO: {
                        glm::vec3 playerPos = glm::vec3(
                                Serializer::decodeFloat(&p->data[0]),
                                Serializer::decodeFloat(&p->data[4]),
                                Serializer::decodeFloat(&p->data[8])
                        );
                        player.setPos(playerPos);
                        break;
                    }
                    case Packet::ENTITY_INFO: {
                        int peer_id = Serializer::decodeInt(&p->data[0]);

                        glm::vec3 playerPos = glm::vec3(
                                Serializer::decodeFloat(&p->data[4]),
                                Serializer::decodeFloat(&p->data[8]),
                                Serializer::decodeFloat(&p->data[12])
                        );

                        bool found = false;
                        for (auto plrEnt : playerEntities) {
                            if (plrEnt->peer_id == peer_id) {
                                plrEnt->setPosition(playerPos);
                                found = true;
                                break;
                            }
                        }

                        if (!found) {
                            playerEntities.push_back(new PlayerEntity(playerPos, peer_id));
                        }
                        break;
                    }
                    case Packet::CHUNK_INFO: {
                        chunkPackets.push_back(p);
                        break;
                    }
                    case Packet::SERVER_INFO: {
                        serverSideChunkGens = Serializer::decodeInt(&p->data[0]);
                        break;
                    }
                    default:
                        break;
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
    Packet p(Packet::PLAYER_INFO);
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