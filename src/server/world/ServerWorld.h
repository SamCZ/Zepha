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
#include "../../def/ServerDefs.h"

class ServerWorld {
public:
    explicit ServerWorld(unsigned int seed, ServerDefs& defs);

    void init();
    void update();

    void addPlayer(ServerPlayer* player);

    void setBlock(glm::vec3 pos, int block);
    int getBlock(glm::vec3 pos);

    ~ServerWorld();
private:
    void playerChangedChunks(ServerPlayer* player);
    void generate(glm::vec3 pos);
    void sendChunk(glm::vec3 pos, ServerPeer& peer);

    std::vector<ServerPlayer*> players;

    WorldGenStream* genStream;
    Dimension dimension;

    std::unordered_set<glm::vec3, VecUtils::compareFunc> generateQueueMap;
    std::vector<glm::vec3> generateQueueList;

    int generatedChunks = 0;
    unsigned int seed;
    ServerDefs& defs;

    //Static vector of chunks to place around players
    std::vector<glm::vec3> generateOrder;
};


#endif //ZEUS_WORLD_H
