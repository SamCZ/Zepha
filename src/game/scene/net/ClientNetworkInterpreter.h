//
// Created by aurailus on 11/01/19.
//

#pragma once

#include <string>
#include <iostream>
#include <glm/vec3.hpp>

#include "../../../util/Timer.h"
#include "../../../util/net/Packet.h"
#include "../../../util/net/NetHandler.h"
#include "../../entity/world/PlayerEntity.h"
#include "../../graph/drawable/DrawableGroup.h"
#include "../world/Player.h"
#include "../world/LocalWorld.h"
#include "../../../util/net/Address.h"
#include "ServerConnection.h"

class ClientNetworkInterpreter {
public:
    ClientNetworkInterpreter(ServerConnection& connection, LocalDefs& defs);
    ClientNetworkInterpreter(const ClientNetworkInterpreter& other) = default;

    void init(LocalWorld* world);
    void update(Player &player);
    void cleanup();

    void setBlock(glm::ivec3 pos, unsigned int block);

    ~ClientNetworkInterpreter();

    std::vector<std::unique_ptr<Packet>> chunkPackets;
    int serverSideChunkGens = 0;
    int recvPackets = 0;
private:
    std::shared_ptr<AtlasRef> playerFrontTex, playerBackTex, shadowTex;
    int id = 0;

    LocalWorld* world = nullptr;
    ServerConnection& connection;
    sptr<Model> playerModel;
};

