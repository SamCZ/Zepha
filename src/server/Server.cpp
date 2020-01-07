//
// Created by aurailus on 09/01/19.
//

#include "Server.h"
#include "../util/net/Deserializer.h"

Server::Server(const std::string& path, unsigned short port, const std::string& subgame) :
    defs(subgame, path),
    clientList(),
    world(10, defs, clientList),
    port(port),
    handler(port, 32),
    config(defs) {

    defs.init(world);
    world.init();
    config.init();

    std::cout << Log::info << "Server started successfully." << Log::endl;
    std::cout << Log::info << "Listening for clients." << Log::endl;
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
            case ENET_EVENT_TYPE_NONE:
                std::cout << "Unknown packet type: " << event.type << std::endl;
                break;

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
                ServerClient* client = static_cast<ServerClient*>(event.peer->data);

                if (client->hasPlayer()) {
                    handlePlayerPacket(*client, p);
                }
                else {
                    bool done = config.handlePacket(*client, p);
                    if (done) {
                        clientList.createPlayer(*client);
                    }
                }

                break;
            }
        }

        enet_packet_destroy(event.packet);
    }

    long sleep_for = interval_ns - loop.elapsedNs();
    if (sleep_for > 0) std::this_thread::sleep_for(std::chrono::nanoseconds(sleep_for));

    deltaTime = loop.elapsedNs() / 1000000.f / 1000.f;
    elapsedSeconds += deltaTime;
}

void Server::handlePlayerPacket(ServerClient &client, Packet& p) {
    //Client *does* have a player, this is ensured in processConnecting().
    ServerPlayer& player = client.getPlayer();

    switch (p.type) {
        default: {
            std::cout << Log::err << "Invalid packet type (" << static_cast<int>(p.type) << ") recieved." << Log::endl;
            break;
        }

        case PacketType::THIS_PLAYER_INFO: {
            Deserializer d(p.data);
            player.setPos(d.read<glm::vec3>());
            player.setAngle(d.read<float>());

            //Send All ServerClients the new positon
            Packet r(PacketType::PLAYER_INFO);
            r.data = Serializer()
                    .append(client.getConnectID())
                    .append(player.getPos())
                    .append(player.getAngle())
                    .data;

            for (auto& iter : clientList.clients) {
                if (iter->getConnectID() != client.getConnectID()) {
                    r.sendTo(iter->getPeer(), PacketChannel::ENTITY);
                }
            }

            break;
        }

        case PacketType::BLOCK_SET: {
            Deserializer d(p.data);

            glm::vec3 pos = d.read<glm::vec3>();
            unsigned int block = d.read<unsigned int>();

            world.setBlock(pos, block);

            if (block == 0) {
                auto def = defs.defs.blockFromId(world.getBlock(pos));
                if (def.callbacks.count(Callback::BREAK)) {
                    def.callbacks[Callback::BREAK](defs.luaApi.vecToTable(pos));
                }
            } else {
                auto def = defs.defs.blockFromId(block);
                if (def.callbacks.count(Callback::PLACE)) {
                    def.callbacks[Callback::PLACE](defs.luaApi.vecToTable(pos));
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