//
// Created by aurailus on 03/07/19.
//

#include "ServerConfig.h"

ServerConfig::ServerConfig(ServerDefs &defs) : defs(defs) {}

void ServerConfig::init() {
    blockIdentifierList.reserve(static_cast<unsigned long>(defs.defs.size()));
    for (unsigned int i = 0; i < defs.defs.size(); i++) {
        blockIdentifierList.push_back(defs.defs.fromId(i).identifier);
    }

    biomeIdentifierList.reserve(static_cast<unsigned long>(defs.biomes.size()));
    for (unsigned int i = 0; i < defs.biomes.size(); i++) {
        biomeIdentifierList.push_back(defs.biomes.biomeFromId(i).identifier);
    }
}

bool ServerConfig::handlePacket(ServerClient &client, Packet &r) {
    switch (r.type) {
        default: break;
        case PacketType::CONNECT_DATA_RECVD: {
            return true;
        }
        case PacketType::BLOCK_IDENTIFIER_LIST: {
            Serializer()
                .append(blockIdentifierList)
                .packet(PacketType::BLOCK_IDENTIFIER_LIST)
                .sendTo(client.getPeer(), PacketChannel::CONNECT);
            break;
        }
        case PacketType::BIOME_IDENTIFIER_LIST: {
            Serializer()
                .append(biomeIdentifierList)
                .packet(PacketType::BIOME_IDENTIFIER_LIST)
                .sendTo(client.getPeer(), PacketChannel::CONNECT);
            break;
        }
        case PacketType::MODS: {
            for (LuaMod& mod : defs.luaApi.mods) {
                Packet p(PacketType::MODS);
                p.data = Serializer().append(mod.serialized).data;
                p.sendTo(client.getPeer(), PacketChannel::CONNECT);
            }

            Packet p(PacketType::MOD_ORDER);
            std::string depends;
            bool delimiter = false;
            for (LuaMod& mod : defs.luaApi.mods) {
                if (delimiter) depends.append(",");
                else delimiter = true;
                depends.append(mod.config.name);
            }
            p.data = Serializer().append(depends).data;
            p.sendTo(client.getPeer(), PacketChannel::CONNECT);
            break;
        }
        case PacketType::MEDIA: {
            const unsigned int MAX_PACKET_SIZE = 32*1024;
            unsigned int packetSize = 0;

            Serializer s {};

            for (ServerTexture& texture : defs.assets.textures) {
                if (packetSize + 20 + texture.data.length() > MAX_PACKET_SIZE && packetSize != 0) {
                    s.append(static_cast<int>(AssetType::END));
                    Packet p(PacketType::MEDIA);

                    p.data = s.data;
                    packetSize = 0;
                    s = {};

                    p.sendTo(client.getPeer(), PacketChannel::CONNECT);
                }

                s.append(static_cast<int>(AssetType::TEXTURE))
                 .append(texture.name)
                 .append(texture.width)
                 .append(texture.height)
                 .append(texture.data);

                packetSize += texture.data.length() + 20;
            }

            for (SerializedModel& model : defs.assets.models) {
                if (packetSize + 16 + model.data.length() > MAX_PACKET_SIZE && packetSize != 0) {
                    s.append(static_cast<int>(AssetType::END));
                    Packet p(PacketType::MEDIA);

                    p.data = s.data;
                    packetSize = 0;
                    s = {};

                    p.sendTo(client.getPeer(), PacketChannel::CONNECT);
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

            p.sendTo(client.getPeer(), PacketChannel::CONNECT);

            Packet d(PacketType::MEDIA_DONE);
            d.sendTo(client.getPeer(), PacketChannel::CONNECT);
        }
    }
    return false;
}
