//
// Created by aurailus on 11/01/19.
//

#include "ClientNetworkInterpreter.h"

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

                switch (p->type) {
                    case PacketType::PLAYER: {
                        id = Serializer::decodeInt(&p->data[0]);
                        auto playerPos = Serializer::decodeFloatVec3(&p->data[4]);
                        player.setPos(playerPos);
                        break;
                    }
                    case PacketType::ENTITY_INFO: {
                        int peer_id = Serializer::decodeInt(&p->data[0]);
                        if (peer_id == id) break;

                        auto playerPos = Serializer::decodeFloatVec3(&p->data[4]);
                        auto playerAngle = Serializer::decodeFloat(&p->data[16]);

                        bool found = false;
                        for (auto& ent : world->dimension.playerEntities) {
                            if (ent.peer_id == peer_id) {
                                ent.interpPos(playerPos);
                                ent.interpAngle(-playerAngle + 90);
                                found = true;
                                break;
                            }
                        }
                        if (!found) world->dimension.playerEntities.emplace_back(playerPos, peer_id, playerModel);
                        break;
                    }
                    case PacketType::BLOCK_SET: {
                        auto pos = Serializer::decodeIntVec3(&p->data[0]);
                        uint block = static_cast<uint>(Serializer::decodeInt(&p->data[12]));
                        world->setBlock(pos, block);
                        break;
                    }
                    case PacketType::CHUNK: {
                        chunkPackets.push_back(std::move(p));
                        break;
                    }
                    case PacketType::SERVER_INFO: {
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
                std::cout << Log::info << "Disconnected from server "
                          << event.peer->address.host << ":" << event.peer->address.port << "." << Log::endl;
                break;
            }

            default:
                break;
        }
    }

    //Send Player Position
    Packet p(PacketType::PLAYER);
    Serializer::encodeFloat(p.data, player.getPos().x);
    Serializer::encodeFloat(p.data, player.getPos().y);
    Serializer::encodeFloat(p.data, player.getPos().z);
    Serializer::encodeFloat(p.data, player.getYaw());
    p.sendTo(connection.getPeer(), PacketChannel::PLAYER);
}

void ClientNetworkInterpreter::cleanup() {
    connection.disconnect();
}

ClientNetworkInterpreter::~ClientNetworkInterpreter() {
    cleanup();
}

void ClientNetworkInterpreter::setBlock(glm::vec3 pos, int block) {
    Packet p(PacketType::BLOCK_SET);
    Serializer::encodeIntVec3(p.data, pos);
    Serializer::encodeInt(p.data, block);
    p.sendTo(connection.getPeer(), PacketChannel::BLOCK);
}