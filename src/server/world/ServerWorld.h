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
#include "../../def/ServerDefs.h"
#include "../conn/ServerClients.h"
#include "../../world/ServerDimension.h"
#include "../../game/scene/world/World.h"

class ServerWorld : public World {
public:
    const static int MB_GEN_H = 6;
    const static int MB_GEN_V = 4;

    explicit ServerWorld(unsigned int seed, ServerDefs& defs, ServerClients& clients);

    void init();
    void update(double delta) override;

    unsigned int getBlock(glm::ivec3 pos) override;
    void setBlock(glm::ivec3 pos, unsigned int block) override;

    ServerDimension dimension {};
private:
    void changedChunks(ServerClient& client);
    bool generateMapBlock(glm::ivec3 pos);

    void sendChunk(const glm::ivec3& pos, ServerClient& client);
    static void sendChunk(const std::shared_ptr<BlockChunk>& chunk, ServerClient& client);
    void sendMapBlock(const glm::ivec3& pos, ServerClient& client);

    static bool isInBounds(glm::ivec3 pos, std::pair<glm::ivec3, glm::ivec3>& bounds);

    std::unique_ptr<WorldGenStream> genStream = nullptr;

    std::unordered_set<glm::vec3, Vec::vec3> generateQueueMap;
    std::vector<glm::vec3> generateQueueList;

    unsigned int seed;
    ServerDefs& defs;
    ServerClients& clientList;

    unsigned int generatedChunks = 0;

    //Static vector of chunks to place around players
    std::vector<glm::vec3> generateOrder;
};

