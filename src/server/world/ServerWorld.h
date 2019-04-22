//
// Created by aurailus on 05/03/19.
//

#ifndef ZEUS_WORLD_H
#define ZEUS_WORLD_H


#include <unordered_map>
#include <unordered_set>
#include "../../world/chunk/BlockChunk.h"
#include "../player/ServerPlayer.h"
#include "WorldGenStream.h"
#include "../../util/Vec.h"
#include "../../world/Dimension.h"

class ServerWorld {
public:
    explicit ServerWorld(unsigned int seed);

    void addPlayer(ServerPlayer* player);
    void update();

    ~ServerWorld() = default;
private:
    void playerChangedChunks(ServerPlayer* player);
    void generate(glm::vec3 pos);
    void sendChunk(glm::vec3 pos, ServerPeer& peer);

    std::vector<ServerPlayer*> players;

    WorldGenStream genStream;
    Dimension dimension;

    std::unordered_set<glm::vec3, VecUtils::compareFunc> generateQueueMap;
    std::vector<glm::vec3> generateQueueList;

    int generatedChunks = 0;

    //Static vector of chunks to place around players
    std::vector<glm::vec3> generateOrder;
};


#endif //ZEUS_WORLD_H
