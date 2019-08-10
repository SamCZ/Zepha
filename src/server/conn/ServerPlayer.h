//
// Created by aurailus on 11/01/19.
//

#ifndef ZEUS_SERVERPLAYER_H
#define ZEUS_SERVERPLAYER_H

#include <glm/vec3.hpp>
#include "../../util/TransPos.h"

class ServerPlayer {
public:
    const static int ACTIVE_RANGE_H = 8;
    const static int ACTIVE_RANGE_V = 8;

    explicit ServerPlayer(glm::vec3 pos, unsigned int connectID, const std::string& username);

    void setPos(glm::vec3 pos);
    void setAngle(float angle);

    glm::vec3 getPos();
    float getAngle();

    glm::vec3 getChunkPos();
    glm::vec3 getLastChunkPos();

    std::pair<glm::vec3, glm::vec3> getChunkBounds();
    std::pair<glm::vec3, glm::vec3> getLastChunkBounds();

    bool changedChunks = true;
private:
    std::string username;

    unsigned int connectID;

    glm::vec3 chunkPos {};
    glm::vec3 lastChunkPos {};
    glm::vec3 pos {};

    float angle = 0;
};


#endif //ZEUS_SERVERPLAYER_H
