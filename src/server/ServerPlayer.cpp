//
// Created by aurailus on 11/01/19.
//

#include <cmath>
#include "ServerPlayer.h"

ServerPlayer::ServerPlayer(ServerPeer *peer) {
    this->peer = peer;
    peer->player = this;
    updateBounds();
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

void ServerPlayer::setPos(glm::vec3 pos) {
    this->lastPos = this->pos;
    this->pos = pos;

    glm::vec3 chunkPos(std::floor(pos.x / 16), std::floor(pos.y / 16), std::floor(pos.z / 16));
    glm::vec3 lastChunkPos(std::floor(lastPos.x / 16), std::floor(lastPos.y / 16), std::floor(lastPos.z / 16));

    if (chunkPos != lastChunkPos) {
        updateBounds();
    }
}

std::pair<glm::vec3, glm::vec3> ServerPlayer::getBounds() {
    return {minBounds, maxBounds};
}

void ServerPlayer::updateBounds() {
    glm::vec3 chunkPos(std::floor(pos.x / 16), std::floor(pos.y / 16), std::floor(pos.z / 16));

    minBounds = glm::vec3(chunkPos.x - ACTIVE_RANGE, chunkPos.y - ACTIVE_RANGE, chunkPos.z - ACTIVE_RANGE);
    maxBounds = glm::vec3(chunkPos.x + ACTIVE_RANGE, chunkPos.y + ACTIVE_RANGE, chunkPos.z + ACTIVE_RANGE);
}

bool ServerPlayer::isInBounds(glm::vec3 pos) {
    return (pos.x >= minBounds.x && pos.x <= maxBounds.x
         && pos.y >= minBounds.y && pos.y <= maxBounds.y
         && pos.z >= minBounds.z && pos.z <= maxBounds.z);
}

ServerPlayer::~ServerPlayer() = default;