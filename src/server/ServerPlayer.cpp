//
// Created by aurailus on 11/01/19.
//

#include <cmath>
#include "ServerPlayer.h"
#include "../generic/helpers/TransPos.h"

ServerPlayer::ServerPlayer(ServerPeer *peer, std::string uuid) {
    this->peer = peer;
    peer->player = this;
    this->username = std::move(uuid);
}

Packet ServerPlayer::getInitPacket() {
    Packet p(Packet::PLAYER_INFO);

    Serializer::encodeFloat(p.data, pos.x);
    Serializer::encodeFloat(p.data, pos.y);
    Serializer::encodeFloat(p.data, pos.z);

    return p;
}

glm::vec3 ServerPlayer::getPos() {
    return pos;
}

glm::vec3 ServerPlayer::getChunkPos() {
    return chunkPos;
}

glm::vec3 ServerPlayer::getOldPos() {
    return lastChunkPos;
}

void ServerPlayer::setPos(glm::vec3 pos) {
    this->pos = pos;
    glm::vec3 newChunkPos = TransPos::chunkFromGlobal(pos);

    if (newChunkPos != chunkPos) {
        if (!changedChunks) {
            lastChunkPos = chunkPos;
            changedChunks = true;
        }
        chunkPos = newChunkPos;
    }
}

std::pair<glm::vec3, glm::vec3> ServerPlayer::getBounds() {
    glm::vec3 minBounds(chunkPos.x - ACTIVE_RANGE, chunkPos.y - ACTIVE_RANGE, chunkPos.z - ACTIVE_RANGE);
    glm::vec3 maxBounds(chunkPos.x + ACTIVE_RANGE, chunkPos.y + ACTIVE_RANGE, chunkPos.z + ACTIVE_RANGE);

    return {minBounds, maxBounds};
}

std::pair<glm::vec3, glm::vec3> ServerPlayer::getOldBounds() {
    glm::vec3 minBounds(lastChunkPos.x - ACTIVE_RANGE, lastChunkPos.y - ACTIVE_RANGE, lastChunkPos.z - ACTIVE_RANGE);
    glm::vec3 maxBounds(lastChunkPos.x + ACTIVE_RANGE, lastChunkPos.y + ACTIVE_RANGE, lastChunkPos.z + ACTIVE_RANGE);

    return {minBounds, maxBounds};
}

bool ServerPlayer::isInBounds(glm::vec3 cPos, std::pair<glm::vec3, glm::vec3> &bounds) {
    return (cPos.x >= bounds.first.x && cPos.x <= bounds.second.x
         && cPos.y >= bounds.first.y && cPos.y <= bounds.second.y
         && cPos.z >= bounds.first.z && cPos.z <= bounds.second.z);
}

std::string ServerPlayer::getUsername() {
    return username;
}

ServerPlayer::~ServerPlayer() = default;