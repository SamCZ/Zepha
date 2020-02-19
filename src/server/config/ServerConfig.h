//
// Created by aurailus on 03/07/19.
//

#pragma once

#include "../conn/ServerClient.h"
#include "../asset/AssetType.h"
#include "../../def/ServerGame.h"
#include "../../util/net/Packet.h"
#include "../../util/net/Serializer.h"

class ServerConfig {
public:
    explicit ServerConfig(ServerGame& defs);

    void init();

    //Bool: Create player
    bool handlePacket(ServerClient &client, Packet& p);
private:
    ServerGame& defs;
    std::vector<std::string> blockIdentifierList {};
    std::vector<std::string> biomeIdentifierList {};
};
