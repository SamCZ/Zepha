//
// Created by aurailus on 03/07/19.
//

#include "ServerConfig.h"

#include "../../PacketView.h"
#include "../../Serializer.h"
#include "../asset/AssetType.h"
#include "../conn/ServerClient.h"
#include "../../../def/ItemDef.h"
#include "../../../def/gen/BiomeDef.h"

ServerConfig::ServerConfig(ServerGame &defs) : game(defs) {}

void ServerConfig::init() {
    blockIdentifierList.reserve(static_cast<unsigned long>(game.defs.size()));
    for (unsigned int i = 0; i < game.defs.size(); i++) {
        blockIdentifierList.push_back(game.defs.fromId(i).identifier);
    }

    biomeIdentifierList.reserve(static_cast<unsigned long>(game.biomes.size()));
    for (unsigned int i = 0; i < game.biomes.size(); i++) {
        biomeIdentifierList.push_back(game.biomes.biomeFromId(i).identifier);
    }
}

bool ServerConfig::handlePacket(ServerClient& client, PacketView& r) {
    switch (r.type) {
        default: break;
        case PacketType::CONNECT_DATA_RECVD: {
            return true;
        }

        case PacketType::SERVER_INFO: {
            Serializer()
                .append(game.biomes.seed)
                .packet(PacketType::SERVER_INFO)
                .sendTo(client.peer, PacketChannel::CONNECT);
            break;
        }

        case PacketType::BLOCK_IDENTIFIER_LIST: {
            Serializer()
                .append(blockIdentifierList)
                .packet(PacketType::BLOCK_IDENTIFIER_LIST)
                .sendTo(client.peer, PacketChannel::CONNECT);
            break;
        }

        case PacketType::BIOME_IDENTIFIER_LIST: {
            Serializer()
                .append(biomeIdentifierList)
                .packet(PacketType::BIOME_IDENTIFIER_LIST)
                .sendTo(client.peer, PacketChannel::CONNECT);
            break;
        }

        case PacketType::MODS: {
            game.parser.sendModsPacket(client.peer);
            break;
        }

        case PacketType::MEDIA: {
            const unsigned int MAX_PACKET_SIZE = 32*1024;
            unsigned int packetSize = 0;

            Serializer s {};

            for (ServerTexture& texture : game.assets.textures) {
                if (packetSize + 20 + texture.data.length() > MAX_PACKET_SIZE && packetSize != 0) {
                    s.append(static_cast<int>(AssetType::END));
                    Packet p(PacketType::MEDIA);

                    p.data = s.data;
                    packetSize = 0;
                    s = {};

                    p.sendTo(client.peer, PacketChannel::CONNECT);
                }

                s.append(static_cast<int>(AssetType::TEXTURE))
                 .append(texture.name)
                 .append(texture.width)
                 .append(texture.height)
                 .append(texture.data);

                packetSize += texture.data.length() + 20;
            }

            for (SerializedModel& model : game.assets.models) {
                if (packetSize + 16 + model.data.length() > MAX_PACKET_SIZE && packetSize != 0) {
                    s.append(static_cast<int>(AssetType::END));
                    Packet p(PacketType::MEDIA);

                    p.data = s.data;
                    packetSize = 0;
                    s = {};

                    p.sendTo(client.peer, PacketChannel::CONNECT);
                }

                s.append(static_cast<int>(AssetType::MODEL))
                 .append(model.name)
                 .append(model.format)
                 .append(model.data);

                packetSize += model.data.length() + 16;
            }

            s.append(static_cast<int>(AssetType::END));
            Packet p(PacketType::MEDIA);
            p.data = s.data;

            p.sendTo(client.peer, PacketChannel::CONNECT);

            Packet d(PacketType::MEDIA_DONE);
            d.sendTo(client.peer, PacketChannel::CONNECT);
        }
    }

    return false;
}
