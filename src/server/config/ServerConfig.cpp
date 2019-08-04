//
// Created by aurailus on 03/07/19.
//

#include "ServerConfig.h"
#include "../../util/net/PacketChannel.h"

ServerConfig::ServerConfig(ServerDefs &defs) : defs(defs) {}

void ServerConfig::init() {
    identifierList.reserve(static_cast<unsigned long>(defs.blocks().definitionsSize()));
    for (int i = 0; i < defs.blocks().definitionsSize(); i++) {
        identifierList.push_back(defs.blocks().fromIndex(i).identifier);
    }
}

bool ServerConfig::handlePacket(ServerClient &client, Packet &r) {
    switch (r.type) {
        default: break;
        case PacketType::CONNECT_DATA_RECVD: {
            return true;
        }
        case PacketType::IDENTIFIER_LIST: {
            Packet p(PacketType::IDENTIFIER_LIST);

            Serializer::encodeInt(p.data, static_cast<int>(identifierList.size()));
            for (auto& str : identifierList) Serializer::encodeString(p.data, str);

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
    }
    return false;
}
