//
// Created by aurailus on 07/02/19.
//

#pragma once

#include <enet/enet.h>
#include <glm/vec3.hpp>
#include <unordered_map>

#include "../../util/Vec.h"
#include "../../game/scene/world/InventoryList.h"
#include "../../game/scene/world/Inventory.h"

class ServerClient {
public:
    const static int CHUNK_SEND_RANGE = 32;

    ServerClient(ENetPeer* peer, ENetAddress address, DefinitionAtlas& defs);

    void setUsername(const std::string& name);

    void setPos(glm::vec3 pos);
    glm::vec3 getPos();

    void setPitch(float pitch);
    float getPitch();

    void setYaw(float yaw);
    float getYaw();

    void setFlying(bool flying);
    bool isFlying();

    void setMapBlockIntegrity(glm::ivec3 pos, unsigned long long integrity);
    unsigned long long getMapBlockIntegrity(glm::ivec3 pos);

    Inventory& getInventory();

    bool hasPlayer = false;

    unsigned int cid;
    std::string username;

    ENetPeer* peer;
    ENetAddress address;

    bool changedMapBlocks = true;
    glm::vec3 lastPos {-10000000, -10000000, -10000000};

private:
    glm::vec3 pos {};
    float pitch = 0;
    float yaw = 0;

    bool flying = false;

    InventoryList hand;
    Inventory inventory;

    std::unordered_map<glm::ivec3, unsigned long long, Vec::ivec3> mapBlockIntegrity {};
};
