//
// Created by aurailus on 29/06/19.
//

#include <iostream>
#include "ServerClient.h"

ServerClient::ServerClient(ENetPeer *peer, ENetAddress address, DefinitionAtlas& defs) :
    peer(peer),
    address(address),
    cid(peer->connectID),
    hand(defs, "hand", 1, 1),
    inventory(defs) {}

void ServerClient::setUsername(const std::string &name) {
    this->username = name;
}

void ServerClient::setPos(glm::vec3 pos) {
    glm::vec3 lastMapBlock = Space::MapBlock::world::fromBlock(this->pos);
    glm::vec3 newMapBlock = Space::MapBlock::world::fromBlock(pos);

    if (newMapBlock != lastMapBlock && !changedMapBlocks) {
        lastMapBlock = newMapBlock;
        changedMapBlocks = true;
    }

    this->pos = pos;
}

glm::vec3 ServerClient::getPos() {
    return pos;
}

void ServerClient::setPitch(float pitch) {
    this->pitch = pitch;
}

float ServerClient::getPitch() {
    return pitch;
}

void ServerClient::setYaw(float yaw) {
    this->yaw = yaw;
}

float ServerClient::getYaw() {
    return yaw;
}

void ServerClient::setFlying(bool flying) {
    this->flying = flying;
}

bool ServerClient::isFlying() {
    return flying;
}

void ServerClient::setMapBlockIntegrity(glm::ivec3 pos, unsigned long long integrity) {
    mapBlockIntegrity[pos] = integrity;
}

unsigned long long ServerClient::getMapBlockIntegrity(glm::ivec3 pos) {
    if (mapBlockIntegrity.count(pos)) return mapBlockIntegrity[pos];
    return 0;
}

Inventory &ServerClient::getInventory() {
    return inventory;
}
