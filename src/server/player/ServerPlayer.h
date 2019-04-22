//
// Created by aurailus on 11/01/19.
//

#ifndef ZEUS_SERVERPLAYER_H
#define ZEUS_SERVERPLAYER_H


#include <vec3.hpp>
#include "../conn/ServerPeer.h"
#include "../../util/net/Packet.h"
#include "../../util/TransPos.h"

class ServerPlayer {
public:
    //This 16 shouldn't be converted to TransPos::CHUNK_SIZE, it's just so that my brain can gauge the distance easier.
    const static int ACTIVE_RANGE_H = (26 * 16) / TransPos::CHUNK_SIZE;
    const static int ACTIVE_RANGE_V = (8 * 16)  / TransPos::CHUNK_SIZE;

    //TODO: Refactor instances of UUID to username, or create seperate username flag
    explicit ServerPlayer(ServerPeer* peer, std::string uuid);

    Packet getInitPacket();

    glm::vec3 getPos();
    void setPos(glm::vec3 pos);

    glm::vec3 getChunkPos();
    glm::vec3 getOldPos();

    bool changedChunks = false;

    std::pair<glm::vec3, glm::vec3> getBounds();
    std::pair<glm::vec3, glm::vec3> getOldBounds();

    bool isInBounds(glm::vec3 pos, std::pair<glm::vec3, glm::vec3>& bounds);

    std::string getUsername();

    ~ServerPlayer();

    ServerPeer* peer;
private:
    std::string username;

    glm::vec3 chunkPos {0, 0, 0};
    glm::vec3 lastChunkPos {0, 0, 0};
    glm::vec3 pos {0, 0, 0};
};


#endif //ZEUS_SERVERPLAYER_H
