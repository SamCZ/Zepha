//
// Created by aurailus on 07/02/19.
//

#pragma once

#include <enet/enet.h>
#include <glm/vec3.hpp>
#include <unordered_map>

#include "../../../game/scene/world/Player.h"

#include "../../../world/ServerDimension.h"
#include "../../../game/inventory/ServerInventory.h"

class ServerClient;
class Deserializer;
class ServerSubgame;
class ServerInventoryRefs;
enum class NetPlayerField;

class ServerPlayer : public Player {
public:
    ServerPlayer(ServerClient& client, SubgamePtr game, DimensionPtr dim);

    virtual void assertField(Packet packet) override;
    virtual void handleAssertion(Deserializer& d) override;

    virtual void setPos(glm::vec3 pos, bool assert = false) override;
    virtual InventoryPtr getInventory() override;

    ENetPeer* getPeer();

    bool changedMapBlocks = true;
    glm::vec3 lastPos = glm::vec3(INFINITY);

private:
    ServerClient& client;
    InventoryPtr inventory;
};
