//
// Created by aurailus on 03/07/19.
//

#include "ServerConfig.h"
#include "../../util/net/PacketChannel.h"

ServerConfig::ServerConfig(ServerDefs &defs) :
    defs(defs) {
}

void ServerConfig::init() {
    identifierList.reserve(static_cast<unsigned long>(defs.blocks().definitionsSize()));
    for (int i = 0; i < defs.blocks().definitionsSize(); i++) {
        identifierList.push_back(defs.blocks().fromIndex(i).getIdentifier());
    }
}

void ServerConfig::handlePacket(ServerClient &client, Packet &r) {
    if (r.type == PacketType::IDENTIFIER_LIST) {

        Packet p(PacketType::IDENTIFIER_LIST);

        Serializer::encodeInt(p.data, static_cast<int>(identifierList.size()));

        for (auto& str : identifierList) {
            Serializer::encodeString(p.data, str);
        }

        p.sendTo(client.getPeer(), PacketChannel::CONNECT_DATA);
    }
}
