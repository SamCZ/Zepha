//
// Created by aurailus on 09/01/19.
//

#include <thread>

#include "../util/Timer.h"

#include "Server.h"

Server::Server(const std::string& path, unsigned short port, const std::string& subgame) :
    defs(subgame, path),
    clientList(defs),
    world(10, defs, clientList),
    port(port),
    handler(port, 32),
    config(defs) {

    defs.init(world);
    world.init();
    config.init();

    std::cout << Log::info << "Server started successfully, listening for clients." << Log::endl;
    while (alive) update();
}

void Server::update() {
    const static long interval_ns = static_cast<long>((1000 / 20.f) * 1000000L);
    Timer loop("");

    world.update(0);
    defs.update(deltaTime, clientList);

    ENetEvent event;
    while (handler.update(&event) && loop.elapsedNs() < interval_ns) {
        switch (event.type) {
            default:
            case ENET_EVENT_TYPE_NONE: {
                std::cout << "Unknown packet type: " << event.type << std::endl;
                break;
            }
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

                if (client->hasPlayer) {
                    handlePlayerPacket(*client, p);
                }
                else {
                    bool done = config.handlePacket(*client, p);
                    if (done) {
                        std::shared_ptr<ServerClient> clientShared = nullptr;
                        for (auto& sClient : clientList.clients) {
                            if (sClient->cid == client->cid) {
                                clientShared = sClient;
                                break;
                            }
                        }
                        if (!clientShared) break;
                        clientList.createPlayer(clientShared);
                    }
                }
                break;
            }
        }

        enet_packet_destroy(event.packet);
    }

    for (auto& cid : playersUpdated) {
        auto client = clientList.getClient(cid);
        if (client == nullptr) continue;

        Packet r(PacketType::PLAYER_INFO, false);
        r.data = Serializer()
                .append(client->cid)
                .append(client->getPos())
                .append(client->getPitch())
                .append(client->getYaw())
                .data;

        for (auto& iter : clientList.clients)
            if (iter->cid != cid && glm::distance(client->getPos(), iter->getPos()) < 200)
                r.sendTo(iter->peer, PacketChannel::ENTITY);
    }
    playersUpdated.clear();

    long sleep_for = interval_ns - loop.elapsedNs();
    if (sleep_for > 0) std::this_thread::sleep_for(std::chrono::nanoseconds(sleep_for));

    deltaTime = loop.elapsedNs() / 1000000.f / 1000.f;
    elapsedSeconds += deltaTime;
}

void Server::handlePlayerPacket(ServerClient &client, Packet& p) {
    switch (p.type) {
        default: {
            std::cout << Log::err << "Invalid packet type (" << static_cast<int>(p.type) << ") recieved." << Log::endl;
            break;
        }
        case PacketType::PLAYER_INFO: {
            Deserializer d(p.data);
            client.setPos(d.read<glm::vec3>());
            client.setPitch(d.read<float>());
            client.setYaw(d.read<float>());

            playersUpdated.emplace(client.cid);
            break;
        }
        case PacketType::BLOCK_SET: {
            Deserializer d(p.data);

            glm::ivec3 pos = d.read<glm::ivec3>();
            unsigned int block = d.read<unsigned int>();

            unsigned int worldBlock = (block == DefinitionAtlas::AIR ? world.getBlock(pos) : 0);

            world.setBlock(pos, block);

            if (block == DefinitionAtlas::AIR) {
                auto def = defs.defs.blockFromId(worldBlock);
                if (def.callbacks.count(Callback::BREAK)) {
                    def.callbacks[Callback::BREAK](defs.parser.vecToTable(pos));
                }
            }
            else {
                auto def = defs.defs.blockFromId(block);
                if (def.callbacks.count(Callback::PLACE)) {
                    def.callbacks[Callback::PLACE](defs.parser.vecToTable(pos));
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