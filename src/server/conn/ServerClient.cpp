//
// Created by aurailus on 29/06/19.
//

#include <iostream>
#include "ServerClient.h"

ServerClient::ServerClient(ENetPeer *peer, ENetAddress address) :
    peer(peer),
    address(address),
    cid(peer->connectID) {}

void ServerClient::setMapBlockIntegrity(glm::ivec3 pos, unsigned long long integrity) {
    mapBlockIntegrity[pos] = integrity;
}

unsigned long long ServerClient::getMapBlockIntegrity(glm::ivec3 pos) {
    if (mapBlockIntegrity.count(pos)) return mapBlockIntegrity[pos];
    return 0;
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

void ServerClient::setAngle(float angle) {
    this->angle = angle;
}

float ServerClient::getAngle() {
    return angle;
}
