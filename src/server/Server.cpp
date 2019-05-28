//
// Created by aurailus on 09/01/19.
//

#include "Server.h"

Server::Server(unsigned short port) :
    world(55),
    connections(&world),
    port(port),
    handler(port, 32) {

    //Some signal to turn off alive is needed to shut down the server gracefully
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
                //TODO: Get an actual username / uuid
                connections.createPlayer(peer, "1234567890", "Aurailus");
                break;
            }
            case ENET_EVENT_TYPE_RECEIVE: {
                Packet p(event.packet);

                auto from = (ServerPeer*)event.peer->data;
                auto player = from->player;

                if (player != nullptr) {
                    if (p.type == Packet::PLAYER_INFO) {

                        //Update Player Object
                        auto newPos = Serializer::decodeFloatVec3(&p.data[0]);
                        auto angle = Serializer::decodeFloat(&p.data[12]);

                        player->setPos(newPos);
                        player->setAngle(angle);

                        //Send All Clients the new positon
                        Packet r(Packet::ENTITY_INFO);

                        Serializer::encodeInt      (r.data, player->peer->index);
                        Serializer::encodeFloatVec3(r.data, newPos);
                        Serializer::encodeFloat    (r.data, angle);

                        for (auto peer : connections.peers) {
                            if (peer->index != player->peer->index)
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