//
// Created by aurailus on 09/01/19.
//

#include "Server.h"
#include "../generic/blocks/BlockChunk.h"

Server::Server() = default;

Server::Server(unsigned short port) {
    this->port = port;
}

void Server::init() {
    handler = NetHandler(port, 32);

    while (alive) update();
}

void Server::update() {
    Timer loop("");

    ENetEvent event;
    while (handler.update(&event) && loop.elapsedNs() < 15L*1000000L) {
        switch (event.type) {
            case ENET_EVENT_TYPE_CONNECT: {
                auto peer = connections.addPeer(event.peer);
                auto player = connections.addPlayer(peer, "Aurailus");
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

                    Packet r(PacketType::ENTITYINFO);

                    Serializer::encodeFloat(r.data, playerPos.x);
                    Serializer::encodeFloat(r.data, playerPos.y - 1.0f);
                    Serializer::encodeFloat(r.data, playerPos.z);

                    r.sendTo(connections.players[0]->peer->peer, PacketChannel::ENTITYINFO);
                }

                enet_packet_destroy(event.packet);
                break;
            }
            case ENET_EVENT_TYPE_DISCONNECT: {
                connections.removePeer(event.peer);
                break;
            }
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