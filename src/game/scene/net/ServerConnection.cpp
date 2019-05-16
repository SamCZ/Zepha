//
// Created by aurailus on 11/01/19.
//

#include "ServerConnection.h"


ServerConnection::ServerConnection(std::string address, unsigned short port, TextureAtlas &atlas) :
    port(port),
    address(std::move(address)) {

    std::string front("player_front");
    std::string back("player_back");
    std::string shadow("player_shadow");
    playerFrontTex = atlas.getTextureRef(front);
    playerBackTex = atlas.getTextureRef(back);
    shadowTex = atlas.getTextureRef(shadow);

    entities = new DrawableGroup();
}

void ServerConnection::init(std::vector<Drawable*> &entities) {
    entities.push_back(this->entities);

    handler = NetHandler(address, port, 3, 3000);

    if (handler.getState() != NetHandler::CLIENT) {
        exit(EXIT_FAILURE);
    }
}

void ServerConnection::update(Player &player) {
    recvPackets = 0;

    ENetEvent event;
    while (handler.update(&event)) {
        recvPackets++;

        switch (event.type) {
            case ENET_EVENT_TYPE_CONNECT: {
                printf("[INFO] Connected to server %x:%u.\n", event.peer->address.host, event.peer->address.port);
                break;
            }
            case ENET_EVENT_TYPE_RECEIVE: {
                Packet p(event.packet);

                switch (p.type) {
                    case Packet::PLAYER_INFO: {
                        this->id = Serializer::decodeInt(&p.data[0]);
                        auto playerPos = Serializer::decodeFloatVec3(&p.data[4]);
                        player.setPos(playerPos);
                        break;
                    }
                    case Packet::ENTITY_INFO: {
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
                    case Packet::CHUNK_INFO: {
                        chunkPackets.push_back(std::move(p));
                        break;
                    }
                    case Packet::SERVER_INFO: {
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
                printf("[INFO] Disconnected from server %x:%u\n", event.peer->address.host, event.peer->address.port);
                break;
            }

            default:
                break;
        }
    }

    //Send Player Position
    Packet p(Packet::PLAYER_INFO);
    Serializer::encodeFloat(p.data, player.getPos().x);
    Serializer::encodeFloat(p.data, player.getPos().y - Player::EYE_HEIGHT);
    Serializer::encodeFloat(p.data, player.getPos().z);
    Serializer::encodeFloat(p.data, player.getYaw());
    p.sendTo(handler.getPeer(), 0);
}

void ServerConnection::cleanup() {
    connected = false;
}

ServerConnection::~ServerConnection() {
    cleanup();
}
