//
// Created by aurailus on 09/01/19.
//

#include "Server.h"
#include "../generic/blocks/BlockChunk.h"

Server::Server() : connections(&world) {};

Server::Server(unsigned short port) : connections(&world) {
    this->port = port;
}

void Server::init() {
    handler = NetHandler(port, 32);

    while (alive) update();
}

void Server::update() {
    Timer loop("");

    world.update();

    ENetEvent event;
    while (handler.update(&event) && loop.elapsedNs() < 15L*1000000L) {
        switch (event.type) {
            case ENET_EVENT_TYPE_CONNECT: {
                auto peer = connections.addPeer(event.peer);
                connections.createPlayer(peer, "Aurailus");
                break;
            }
            case ENET_EVENT_TYPE_RECEIVE: {
                Packet p(event.packet);

                auto from = (ServerPeer*)event.peer->data;
                auto player = from->player;

                if (player != nullptr) {
                    if (p.type == Packet::PLAYER_INFO) {

                        //Update Player Object
                        glm::vec3 newPos = glm::vec3(
                                Serializer::decodeFloat(&p.data[0]),
                                Serializer::decodeFloat(&p.data[4]),
                                Serializer::decodeFloat(&p.data[8])
                        );
                        player->setPos(newPos);

                        //Send All Clients the new positon
                        Packet r(Packet::ENTITY_INFO);

                        Serializer::encodeInt  (r.data, player->peer->index);
                        Serializer::encodeFloat(r.data, newPos.x);
                        Serializer::encodeFloat(r.data, newPos.y - 1.0f);
                        Serializer::encodeFloat(r.data, newPos.z);

                        for (auto peer : connections.peers) {
                            r.sendTo(peer->peer, PacketChannel::ENTITY_INFO);
                        }
                    }
                }

                enet_packet_destroy(event.packet);
                break;
            }
            case ENET_EVENT_TYPE_DISCONNECT: {
                connections.removePeer(event.peer);
                break;
            }
            case ENET_EVENT_TYPE_NONE:
            default:
                break;
        }
    }

    long sleep_for = 16L*1000000L - loop.elapsedNs();
    std::this_thread::sleep_for(std::chrono::nanoseconds(sleep_for));
}

void Server::cleanup() {
    alive = false;
}

Server::~Server() {
    cleanup();
}