//
// Created by aurailus on 03/07/19.
//

#ifndef ZEUS_SERVERCONFIG_H
#define ZEUS_SERVERCONFIG_H

#include "../conn/ServerClient.h"
#include "../../def/ServerDefs.h"
#include "../../util/net/Packet.h"

class ServerConfig {
public:
    explicit ServerConfig(ServerDefs& defs);

    void init();

    //Bool: Create player
    bool handlePacket(ServerClient &client, Packet& p);
private:
    ServerDefs& defs;
    std::vector<std::string> identifierList {};
};

#endif //ZEUS_SERVERCONFIG_H
