//
// Created by aurailus on 11/01/19.
//

#include "ClientNetworkInterpreter.h"

ClientNetworkInterpreter::ClientNetworkInterpreter(ServerConnection &connection, LocalDefs &defs) :
    connection(connection),
    entities(new DrawableGroup()),
    world(nullptr),

    playerFrontTex(defs.textures().getTextureRef("player_front")),
    playerBackTex(defs.textures().getTextureRef("player_back")),
    shadowTex(defs.textures().getTextureRef("player_shadow")) {}

void ClientNetworkInterpreter::init(std::vector<Drawable *> &entities, LocalWorld *world) {
    entities.push_back(this->entities);
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
                          << event.peer->address.host << ":" << event.peer->address.port << "." << Log::endl;
                break;
            }
            case ENET_EVENT_TYPE_RECEIVE: {
                Packet p(event.packet);

                switch (p.type) {
                    case PacketType::PLAYER: {
                        id = Serializer::decodeInt(&p.data[0]);
                        auto playerPos = Serializer::decodeFloatVec3(&p.data[4]);
                        player.setPos(playerPos);
                        break;
                    }
                    case PacketType::ENTITY_INFO: {
                        int peer_id = Serializer::decodeInt(&p.data[0]);
                        if (peer_id == id) break;

                        auto playerPos = Serializer::decodeFloatVec3(&p.data[4]);
                        auto playerAngle = Serializer::decodeFloat(&p.data[16]);

                        bool found = false;
                        for (auto ent : entities->getChildren()) {
                            auto playerEntity = (PlayerEntity*)ent;

                            if (playerEntity->peer_id == peer_id) {

                                playerEntity->setPos(playerPos);
                                playerEntity->setAngle(-playerAngle);

                                found = true;
                                break;
                            }
                        }

                        if (!found) {
                            entities->addDrawable(new PlayerEntity(playerPos, peer_id, playerFrontTex, playerBackTex, shadowTex));
                        }
                        break;
                    }
                    case PacketType::BLOCK_SET: {
                        auto pos = Serializer::decodeIntVec3(&p.data[0]);
                        auto block = Serializer::decodeInt(&p.data[12]);
                        world->setBlock(pos, block);
                        break;
                    }
                    case PacketType::CHUNK: {
                        chunkPackets.push_back(std::move(p));
                        break;
                    }
                    case PacketType::SERVER_INFO: {
                        serverSideChunkGens = Serializer::decodeInt(&p.data[0]);
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
    Serializer::encodeFloat(p.data, player.getPos().y - Player::EYE_HEIGHT);
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