//
// Created by aurailus on 11/01/19.
//

#include <iostream>
#include "ServerPlayer.h"

ServerPlayer::ServerPlayer(glm::vec3 pos, unsigned int connectID, const std::string& username) :
    pos(pos),
    connectID(connectID),
    username(username) {

    mapBlock = TransPos::mapBlockFromVec(pos);
    lastMapBlock = mapBlock - glm::vec3(200); //Force resending of all chunks around getPlayer.
    changedMapBlocks = true;
}

void ServerPlayer::setPos(glm::vec3 pos) {
    this->pos = pos;
    glm::vec3 newMapBlock = TransPos::mapBlockFromVec(pos);

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
    return TransPos::chunkFromVec(pos);
}