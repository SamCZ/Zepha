//
// Created by aurailus on 11/01/19.
//

#include <iostream>
#include "ServerPlayer.h"

ServerPlayer::ServerPlayer(glm::vec3 pos, unsigned int connectID, const std::string& username) :
    pos(pos),
    connectID(connectID),
    username(username) {

    mapBlock = Space::MapBlock::world::fromBlock(pos);
    lastMapBlock = mapBlock - glm::vec3(200); //Force resending of all chunks around getPlayer.
    changedMapBlocks = true;
}

void ServerPlayer::setPos(glm::vec3 pos) {
    this->pos = pos;
    glm::vec3 newMapBlock = Space::MapBlock::world::fromBlock(pos);

    if (newMapBlock != mapBlock) {
        if (!changedMapBlocks) {
            lastMapBlock = mapBlock;
            changedMapBlocks = true;
        }
        mapBlock = newMapBlock;
    }
}

void ServerPlayer::setAngle(float angle) {
    this->angle = angle;
}

glm::vec3 ServerPlayer::getPos() {
    return pos;
}

float ServerPlayer::getAngle() {
    return angle;
}

glm::vec3 ServerPlayer::getChunkPos() {
    return Space::Chunk::world::fromBlock(pos);
}

void ServerPlayer::setMapBlockIntegrity(glm::vec3 pos, unsigned long long integrity) {
    mapBlockIntegrity[pos] = integrity;
}

unsigned long long ServerPlayer::getMapBlockIntegrity(glm::vec3 pos) {
    if (mapBlockIntegrity.count(pos)) return mapBlockIntegrity[pos];
    return 0;
}
