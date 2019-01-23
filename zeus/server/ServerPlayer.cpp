//
// Created by aurailus on 11/01/19.
//

#include "ServerPlayer.h"

ServerPlayer::ServerPlayer() {
    this->connection = nullptr;
}

ServerPlayer::ServerPlayer(ClientConnection *connection, glm::vec3 pos) {
    this->pos = pos;
    this->connection = connection;
}
