//
// Created by aurailus on 11/01/19.
//

#pragma once

#include <glm/vec3.hpp>
#include <unordered_map>
#include "../../util/Vec.h"
#include "../../util/Space.h"

class ServerPlayer {
public:
    const static int CHUNK_SEND_RANGE = 32;

    explicit ServerPlayer(glm::vec3 pos, unsigned int connectID, const std::string& username);

    void setPos(glm::vec3 pos);
    void setAngle(float angle);

    glm::vec3 getPos();
    glm::vec3 getChunkPos();
    float getAngle();

    glm::vec3 mapBlock;
    glm::vec3 lastMapBlock;
    bool changedMapBlocks = true;

    void setMapBlockIntegrity(glm::vec3 pos, unsigned long long integrity);
    unsigned long long getMapBlockIntegrity(glm::vec3 pos);
private:
    std::string username;
    unsigned int connectID;

    glm::vec3 pos {};
    float angle = 0;

    std::unordered_map<glm::vec3, unsigned long long, Vec::vec3> mapBlockIntegrity {};
};

