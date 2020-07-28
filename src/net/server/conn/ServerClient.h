//
// Created by aurailus on 07/02/19.
//

#pragma once

#include <enet/enet.h>
#include <glm/vec3.hpp>
#include <unordered_map>

#include "../../../util/Vec.h"
#include "../../../def/DefinitionAtlas.h"
#include "../../../game/inventory/Inventory.h"
#include "../../../game/inventory/ServerInventoryList.h"

enum class NetPlayerField;
class Deserializer;
class InventoryRefs;

class ServerClient {
public:
    const static int CHUNK_SEND_RANGE = 32;

    ServerClient(ENetPeer* peer, ENetAddress address, InventoryRefs& refs);

    void setUsername(const std::string& name);

    glm::vec3 getPos();
    void setPos(glm::vec3 pos, bool assert = false);

    glm::vec3 getVel();
    void setVel(glm::vec3 vel, bool assert = false);

    float getPitch();
    void setPitch(float pitch, bool assert = false);

    float getYaw();
    void setYaw(float yaw, bool assert = false);

    bool isFlying();
    void setFlying(bool flying, bool assert = false);

    std::shared_ptr<Inventory> getInventory();

    std::shared_ptr<InventoryList> getHandList();
    void setHandList(const std::string& list, bool assert = false);

    std::shared_ptr<InventoryList> getWieldList();
    void setWieldList(const std::string& list, bool assert = false);

    unsigned short getWieldIndex();
    void setWieldIndex(unsigned short index, bool assert = false);

    void handleAssertion(Deserializer& d);

//    void setMapBlockIntegrity(glm::ivec3 pos, unsigned long long integrity);
//    unsigned long long getMapBlockIntegrity(glm::ivec3 pos);

    bool hasPlayer = false;

    unsigned int cid = 0;
    std::string username;

    ENetPeer* peer;
    ENetAddress address;

    bool changedMapBlocks = true;
    glm::vec3 lastPos = glm::vec3(INFINITY);

private:
    template <typename T> void assertField(NetPlayerField field, T data);

    glm::vec3 pos { 0, -37, 0 };
    glm::vec3 vel {};
    float pitch = 0;
    float yaw = 0;

    bool flying = false;

    std::shared_ptr<Inventory> inventory;
    unsigned int wieldIndex = 0;

    std::string handList = "";
    std::string wieldList = "";

    unsigned int handItem = DefinitionAtlas::AIR;
    unsigned int wieldItem = DefinitionAtlas::AIR;

    std::unordered_map<glm::ivec3, unsigned long long, Vec::ivec3> mapBlockIntegrity {};
};
