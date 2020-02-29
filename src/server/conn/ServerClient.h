//
// Created by aurailus on 07/02/19.
//

#pragma once

#include <enet/enet.h>
#include <glm/vec3.hpp>
#include <unordered_map>

#include "../../util/Vec.h"
#include "../../game/inventory/ServerInventoryList.h"
#include "../../game/inventory/Inventory.h"

class InventoryRefs;

class ServerClient {
public:
    const static int CHUNK_SEND_RANGE = 32;

    ServerClient(ENetPeer* peer, ENetAddress address, DefinitionAtlas& defs, InventoryRefs& refs);

    void setUsername(const std::string& name);

    void assertPos(glm::vec3 pos);
    void setPos(glm::vec3 pos);
    glm::vec3 getPos();

    void assertPitch(float pitch);
    void setPitch(float pitch);
    float getPitch();

    void assertYaw(float yaw);
    void setYaw(float yaw);
    float getYaw();

    void setFlying(bool flying);
    bool isFlying();

    void setMapBlockIntegrity(glm::ivec3 pos, unsigned long long integrity);
    unsigned long long getMapBlockIntegrity(glm::ivec3 pos);

    std::shared_ptr<Inventory> getInventory();

    bool hasPlayer = false;

    unsigned int cid = 0;
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

    std::shared_ptr<Inventory> inventory;

    std::unordered_map<glm::ivec3, unsigned long long, Vec::ivec3> mapBlockIntegrity {};
};
