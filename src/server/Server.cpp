//
// Created by aurailus on 09/01/19.
//

#include "Server.h"

Server::Server(unsigned short port) :
    defs(),
    clientList(),
    world(55, defs, clientList),
    port(port),
    handler(port, 32),
    config(defs) {

    defs.init(world);
    world.init();
    config.init();

    while (alive) update();
}

void Server::update() {
    const static long interval_ns = (long)((1000 / 20.f) * 1000000L);
    Timer loop("");

    world.update();
    defs.update(deltaTime);

    ENetEvent event;
    while (handler.update(&event) && loop.elapsedNs() < interval_ns) {
        switch (event.type) {
            default:
            case ENET_EVENT_TYPE_NONE: break;

            case ENET_EVENT_TYPE_CONNECT: {
                clientList.handleConnect(event);
                break;
            }

            case ENET_EVENT_TYPE_DISCONNECT: {
                clientList.handleDisconnect(event);
                break;
            }

            case ENET_EVENT_TYPE_RECEIVE: {
                Packet p(event.packet);
                ServerClient *client = (ServerClient *) event.peer->data;

                if (client->hasPlayer()) {
                    handlePlayerPacket(*client, p);
                }
                else {
                    config.handlePacket(*client, p);
                }

                break;
            }
        }

        enet_packet_destroy(event.packet);
    }

    long sleep_for = interval_ns - loop.elapsedNs();
    if (sleep_for > 0) std::this_thread::sleep_for(std::chrono::nanoseconds(sleep_for));

    deltaTime = loop.elapsedNs() / 1000000.0;
    elapsedSeconds += deltaTime;
}

void Server::handlePlayerPacket(ServerClient &client, Packet& p) {
    //Client *does* have a player, this is ensured in update().
    ServerPlayer& player = client.getPlayer();

    switch (p.type) {
        default: {
            std::cout << Log::err << "Invalid packet type (" << static_cast<int>(p.type) << ") recieved." << Log::endl;
            break;
        }

        case PacketType::PLAYER_INFO: {
            player.setPos(Serializer::decodeFloatVec3(&p.data[0]));
            player.setAngle(Serializer::decodeFloat(&p.data[12]));

            //Send All ServerClients the new positon
            Packet r(PacketType::ENTITY_INFO);

            Serializer::encodeInt(r.data, client.getConnectID());
            Serializer::encodeFloatVec3(r.data, player.getPos());
            Serializer::encodeFloat(r.data, player.getAngle());

            for (auto& iter : clientList.clients) {
                if (iter.getConnectID() != client.getConnectID()) {
                    r.sendTo(iter.getPeer(), PacketChannel::ENTITY_INFO);
                }
            }

            break;
        }

        case PacketType::BLOCK_SET: {
            auto pos = Serializer::decodeIntVec3(&p.data[0]);
            auto block = Serializer::decodeInt(&p.data[12]);

            world.setBlock(pos, block);

            if (block == 0) {
                auto def = defs.blocks().fromIndex(world.getBlock(pos));
                if (def.callbacks.count(Callback::BREAK)) {
                    def.callbacks[Callback::BREAK](defs.lua().vecToTable(pos));
                }
            } else {
                auto def = defs.blocks().fromIndex(block);
                if (def.callbacks.count(Callback::PLACE)) {
                    def.callbacks[Callback::PLACE](defs.lua().vecToTable(pos));
                }
            }

            break;
        }
    }
}

void Server::cleanup() {
    alive = false;
}

Server::~Server() {
    cleanup();
}