//
// Created by aurailus on 11/01/19.
//

#include "ServerPlayer.h"

ServerPlayer::ServerPlayer(glm::vec3 pos, unsigned int connectID, std::string username) :
    pos(pos),
    connectID(connectID),
    username(std::move(username)) {

    chunkPos = TransPos::chunkFromVec(pos);
    lastChunkPos = chunkPos - glm::vec3(200); //Force resending of all chunks around getPlayer.
    changedChunks = true;
}

void ServerPlayer::setPos(glm::vec3 pos) {
    this->pos = pos;
    glm::vec3 newChunkPos = TransPos::chunkFromVec(pos);

    if (newChunkPos != chunkPos) {
        if (!changedChunks) {
            lastChunkPos = chunkPos;
            changedChunks = true;
        }
        chunkPos = newChunkPos;
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
    return chunkPos;
}

glm::vec3 ServerPlayer::getLastChunkPos() {
    return lastChunkPos;
}

std::pair<glm::vec3, glm::vec3> ServerPlayer::getChunkBounds() {
    glm::vec3 minBounds(chunkPos.x - ACTIVE_RANGE_H, chunkPos.y - ACTIVE_RANGE_V, chunkPos.z - ACTIVE_RANGE_H);
    glm::vec3 maxBounds(chunkPos.x + ACTIVE_RANGE_H, chunkPos.y + ACTIVE_RANGE_V, chunkPos.z + ACTIVE_RANGE_H);

    return {minBounds, maxBounds};
}

std::pair<glm::vec3, glm::vec3> ServerPlayer::getLastChunkBounds() {
    glm::vec3 minBounds(lastChunkPos.x - ACTIVE_RANGE_H, lastChunkPos.y - ACTIVE_RANGE_V, lastChunkPos.z - ACTIVE_RANGE_H);
    glm::vec3 maxBounds(lastChunkPos.x + ACTIVE_RANGE_H, lastChunkPos.y + ACTIVE_RANGE_V, lastChunkPos.z + ACTIVE_RANGE_H);

    return {minBounds, maxBounds};
}