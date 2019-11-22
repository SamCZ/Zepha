//
// Created by aurailus on 11/01/19.
//

#pragma once

#include <glm/vec3.hpp>
#include "../../util/Space.h"

class ServerPlayer {
public:
    const static int CHUNK_SEND_RANGE = 12;

    explicit ServerPlayer(glm::vec3 pos, unsigned int connectID, const std::string& username);

    void setPos(glm::vec3 pos);
    void setAngle(float angle);

    glm::vec3 getPos();
    glm::vec3 getChunkPos();
    float getAngle();

    glm::vec3 mapBlock;
    glm::vec3 lastMapBlock;
    bool changedMapBlocks = true;
private:
    std::string username;
    unsigned int connectID;

    glm::vec3 pos {};
    float angle = 0;
};

