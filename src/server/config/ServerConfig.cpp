//
// Created by aurailus on 03/07/19.
//

#include "ServerConfig.h"

ServerConfig::ServerConfig(ServerDefs &defs) :
    defs(defs) {

}

void ServerConfig::handlePacket(ServerClient &client, Packet &p) {
    if (p.type == PacketType::MOD_DATA) {
        return;
    }
}
