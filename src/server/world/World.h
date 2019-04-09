//
// Created by aurailus on 05/03/19.
//

#ifndef ZEUS_WORLD_H
#define ZEUS_WORLD_H


#include <unordered_map>
#include <unordered_set>
#include "../../generic/blocks/BlockChunk.h"
#include "../ServerPlayer.h"
#include "WorldGenStream.h"
#include "../../generic/helpers/VecUtils.h"
#include "../../generic/world/Dimension.h"

class World {
public:
    explicit World(unsigned int seed);

    void addPlayer(ServerPlayer* player);
    void update();

    ~World() = default;
private:
    void playerChangedChunks(ServerPlayer* player);
    void generate(glm::vec3 pos);
    void sendChunk(glm::vec3 pos, ServerPeer& peer);

    WorldGenStream genStream;

    std::vector<ServerPlayer*> players;

    Dimension dimension;

    std::unordered_set<glm::vec3, VecUtils::compareFunc> generateQueueMap;
    std::vector<glm::vec3> generateQueueList;

    int generatedChunks = 0;

    std::vector<glm::vec3> generateOrder;
};


#endif //ZEUS_WORLD_H
