//
// Created by aurailus on 03/07/19.
//

#include "ServerConfig.h"
#include "../../util/net/PacketChannel.h"
#include "../asset/AssetType.h"

ServerConfig::ServerConfig(ServerDefs &defs) : defs(defs) {}

void ServerConfig::init() {
    blockIdentifierList.reserve(static_cast<unsigned long>(defs.defs().size()));
    for (unsigned int i = 0; i < defs.defs().size(); i++) {
        blockIdentifierList.push_back(defs.defs().fromId(i).identifier);
    }

    biomeIdentifierList.reserve(static_cast<unsigned long>(defs.gen().size()));
    for (unsigned int i = 0; i < defs.gen().size(); i++) {
        biomeIdentifierList.push_back(defs.gen().biomeFromId(i).identifier);
    }
}

bool ServerConfig::handlePacket(ServerClient &client, Packet &r) {
    switch (r.type) {
        default: break;
        case PacketType::CONNECT_DATA_RECVD: {
            return true;
        }
        case PacketType::BLOCK_IDENTIFIER_LIST: {
            Packet p(PacketType::BLOCK_IDENTIFIER_LIST);

            Serializer::encodeUInt(p.data, static_cast<int>(blockIdentifierList.size()));
            for (auto& str : blockIdentifierList) Serializer::encodeString(p.data, str);

            p.sendTo(client.getPeer(), PacketChannel::CONNECT);
            break;
        }
        case PacketType::BIOME_IDENTIFIER_LIST: {
            Packet p(PacketType::BIOME_IDENTIFIER_LIST);

            Serializer::encodeUInt(p.data, static_cast<int>(biomeIdentifierList.size()));
            for (auto& str : biomeIdentifierList) Serializer::encodeString(p.data, str);

            p.sendTo(client.getPeer(), PacketChannel::CONNECT);
            break;
        }
        case PacketType::MODS: {
            for (LuaMod& mod : defs.lua().mods) {
                Packet p(PacketType::MODS);
                Serializer::encodeString(p.data, mod.serialized);
                p.sendTo(client.getPeer(), PacketChannel::CONNECT);
            }

            Packet p(PacketType::MOD_ORDER);
            std::string depends;
            bool delimiter = false;
            for (LuaMod& mod : defs.lua().mods) {
                if (delimiter) depends.append(",");
                else delimiter = true;
                depends.append(mod.config.name);
            }
            Serializer::encodeString(p.data, depends);
            p.sendTo(client.getPeer(), PacketChannel::CONNECT);
            break;
        }
        case PacketType::MEDIA: {
            const unsigned int MAX_PACKET_SIZE = 32*1024;
            unsigned int packetSize = 0;

            Packet p(PacketType::MEDIA);

            for (ServerTexture& texture : defs.assets().textures) {
                if (packetSize + 20 + texture.data.length() > MAX_PACKET_SIZE && packetSize != 0) {
                    Serializer::encodeInt(p.data, static_cast<int>(AssetType::END));
                    p.sendTo(client.getPeer(), PacketChannel::CONNECT);
                    p = Packet(PacketType::MEDIA);
                    packetSize = 0;
                }

                Serializer::encodeInt(p.data, static_cast<int>(AssetType::TEXTURE));
                Serializer::encodeString(p.data, texture.name);
                Serializer::encodeInt(p.data, texture.width);
                Serializer::encodeInt(p.data, texture.height);
                Serializer::encodeString(p.data, texture.data);
                packetSize += texture.data.length() + 20;
            }

            for (SerializedModel& model : defs.assets().models) {
                if (packetSize + 16 + model.data.length() > MAX_PACKET_SIZE && packetSize != 0) {
                    Serializer::encodeInt(p.data, static_cast<int>(AssetType::END));
                    p.sendTo(client.getPeer(), PacketChannel::CONNECT);
                    p = Packet(PacketType::MEDIA);
                    packetSize = 0;
                }

                Serializer::encodeInt(p.data, static_cast<int>(AssetType::MODEL));
                Serializer::encodeString(p.data, model.name);
                Serializer::encodeString(p.data, model.format);
                Serializer::encodeString(p.data, model.data);
                packetSize += model.data.length() + 16;
            }

            Serializer::encodeInt(p.data, static_cast<int>(AssetType::END));
            p.sendTo(client.getPeer(), PacketChannel::CONNECT);

            Packet d(PacketType::MEDIA_DONE);
            d.sendTo(client.getPeer(), PacketChannel::CONNECT);
        }
    }
    return false;
}
