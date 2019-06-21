//
// Created by aurailus on 09/01/19.
//

#include "Server.h"

Server::Server(unsigned short port) :
    defs(),
    world(55, defs),
    connections(&world),
    port(port),
    handler(port, 32) {

    defs.init(world);
    world.init();

    //Some signal to turn off alive is needed to shut down the server gracefully
    while (alive) update();
}

void Server::update() {
    Timer loop("");

    world.update();
    defs.update(16); //TODO: Calculate the real delta.

    ENetEvent event;
    while (handler.update(&event) && loop.elapsedNs() < 15L*1000000L) {
        switch (event.type) {
            case ENET_EVENT_TYPE_NONE:
            default: break;

            case ENET_EVENT_TYPE_CONNECT: {
                auto peer = connections.addPeer(event.peer);
                //TODO: Get an actual username / uuid
                connections.createPlayer(peer, "1234567890", "Aurailus");
                break;
            }

            case ENET_EVENT_TYPE_DISCONNECT: {
                connections.removePeer(event.peer);
                break;
            }

            case ENET_EVENT_TYPE_RECEIVE: {
                Packet p(event.packet);

                auto from = (ServerPeer*)event.peer->data;
                auto player = from->player;

                if (player != nullptr) {
                    switch (p.type) {
                        default: break;

                        case Packet::PLAYER_INFO: {
                            //Update Player Object
                            auto newPos = Serializer::decodeFloatVec3(&p.data[0]);
                            auto angle = Serializer::decodeFloat(&p.data[12]);

                            player->setPos(newPos);
                            player->setAngle(angle);

                            //Send All Clients the new positon
                            Packet r(Packet::ENTITY_INFO);

                            Serializer::encodeInt(r.data, player->peer->index);
                            Serializer::encodeFloatVec3(r.data, newPos);
                            Serializer::encodeFloat(r.data, angle);

                            for (auto peer : connections.peers) {
                                if (peer->index != player->peer->index)
                                    r.sendTo(peer->peer, PacketChannel::ENTITY_INFO);
                            }

                            break;
                        }

                        case Packet::BLOCK_SET: {
                            auto pos = Serializer::decodeIntVec3(&p.data[0]);
                            auto block = Serializer::decodeInt(&p.data[12]);

                            world.setBlock(pos, block);

                            if (block == 0) {
                                auto def = defs.blocks().fromIndex(world.getBlock(pos));
                                if (def.callbacks.count(Callback::BREAK)) {
                                    def.callbacks[Callback::BREAK](defs.lua().vecToTable(pos));
                                }
                            }
                            else {
                                auto def = defs.blocks().fromIndex(block);
                                if (def.callbacks.count(Callback::PLACE)) {
                                    def.callbacks[Callback::PLACE](defs.lua().vecToTable(pos));
                                }
                            }

                            break;
                        }
                    }
                }

                enet_packet_destroy(event.packet);
                break;
            }
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