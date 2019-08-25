//
// Created by aurailus on 05/03/19.
//

#pragma once

#include <unordered_map>
#include <unordered_set>
#include "WorldGenStream.h"
#include "../conn/ServerClient.h"
#include "../conn/ServerPlayer.h"
#include "../../world/chunk/BlockChunk.h"
#include "../../util/Vec.h"
#include "../../world/Dimension.h"
#include "../../def/ServerDefs.h"
#include "../conn/ServerClients.h"

class ServerWorld {
public:
    explicit ServerWorld(unsigned int seed, ServerDefs& defs, ServerClients& clients);

    void init();
    void update();

    void setBlock(glm::vec3 pos, unsigned int block);
    unsigned int getBlock(glm::vec3 pos);

    ~ServerWorld();
private:
    void changedChunks(ServerClient& client);
    void generate(glm::vec3 pos);
    void sendChunk(glm::vec3 pos, ServerClient& client);

    bool isInBounds(glm::vec3 pos, std::pair<glm::vec3, glm::vec3>& bounds);

    WorldGenStream* genStream = nullptr;
    Dimension dimension;

    std::unordered_set<glm::vec3, VecUtils::compareFunc> generateQueueMap;
    std::vector<glm::vec3> generateQueueList;

    unsigned int seed;
    ServerDefs& defs;
    ServerClients& clientList;

    int generatedChunks = 0;

    //Static vector of chunks to place around players
    std::vector<glm::vec3> generateOrder;
};

