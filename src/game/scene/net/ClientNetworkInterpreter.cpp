//
// Created by aurailus on 11/01/19.
//

#include "ClientNetworkInterpreter.h"
#include "../../../util/net/Serializer.h"

ClientNetworkInterpreter::ClientNetworkInterpreter(ServerConnection &connection, LocalDefs &defs) :
    connection(connection),
    world(nullptr),
    playerModel(std::make_shared<Model>()){
    playerModel->fromSerialized(defs.models.models["zeus:default:player"], {defs.textures["zeus:default:player"]});
}

void ClientNetworkInterpreter::init(LocalWorld *world) {
    this->world = world;
}

void ClientNetworkInterpreter::update(Player &player) {
    recvPackets = 0;

    ENetEvent event;
    while (connection.pollEvents(&event)) {
        recvPackets++;

        switch (event.type) {
            case ENET_EVENT_TYPE_CONNECT: {
                std::cout << Log::info << "Connected to server "
                          << NetHandler::intToIPString(event.peer->address.host)
                          << ":" << event.peer->address.port << "." << Log::endl;
                break;
            }
            case ENET_EVENT_TYPE_RECEIVE: {
                std::unique_ptr<Packet> p = std::make_unique<Packet>(event.packet);
                Deserializer d(p->data);

                switch (p->type) {
                    case PacketType::THIS_PLAYER_INFO: {
                        id = d.read<unsigned int>();

                        player.setPos(d.read<glm::vec3>());
                        player.setYaw(d.read<float>());
                        break;
                    }
                    case PacketType::PLAYER_INFO: {
                        unsigned int peer_id = d.read<unsigned int>();
                        if (peer_id == id) break;

                        bool found = false;
                        for (auto& ent : world->dimension.playerEntities) {
                            if (ent.peer_id == peer_id) {
                                ent.interpPos(d.read<glm::vec3>());
                                ent.interpRotateY(-d.read<float>() + 90);
                                found = true;
                                break;
                            }
                        }
                        if (!found) world->dimension.playerEntities.emplace_back(d.read<glm::vec3>(), peer_id, playerModel);
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
                    default:
                        std::cout << Log::err << "Received unknown packet of type " << static_cast<int>(p->type)
                                  << ". Is the server on a different protocol version?" << Log::endl;
                        break;
                }

                enet_packet_destroy(event.packet);
                break;
            }
            case ENET_EVENT_TYPE_DISCONNECT: {
                std::cout << Log::info << "Disconnected from server "
                          << event.peer->address.host << ":" << event.peer->address.port << "." << Log::endl;
                break;
            }

            default:
                break;
        }
    }

    //Send Player Position
    Packet p(PacketType::THIS_PLAYER_INFO);
    p.data = Serializer()
            .append(player.getPos())
            .append(player.getYaw())
            .data;

    p.sendTo(connection.getPeer(), PacketChannel::PLAYER);
}

void ClientNetworkInterpreter::cleanup() {
    connection.disconnect();
}

ClientNetworkInterpreter::~ClientNetworkInterpreter() {
    cleanup();
}

void ClientNetworkInterpreter::setBlock(glm::ivec3 pos, unsigned int block) {
    Packet p(PacketType::BLOCK_SET);
    Serializer()
        .append(pos)
        .append(block)
        .packet(PacketType::BLOCK_SET)
        .sendTo(connection.getPeer(), PacketChannel::BLOCK);
}