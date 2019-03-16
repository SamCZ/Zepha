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

class World {
public:
    World() = default;

    void addPlayer(ServerPlayer* player);
    void update();

    ~World() = default;
private:
    void playerChangedChunks(ServerPlayer* player);
    void generate(glm::vec3 pos);

    struct vec3cmp {
        size_t operator()(const glm::vec3& k)const {
            return std::hash<float>()(k.x) ^ std::hash<float>()(k.y) ^ std::hash<float>()(k.z);
        }
    };

    WorldGenStream genStream;

    std::vector<ServerPlayer*> players;

    std::unordered_map<glm::vec3, BlockChunk*, vec3cmp> chunkMap;
    std::vector<std::pair<glm::vec3, BlockChunk*>> chunkList;

    std::unordered_set<glm::vec3, vec3cmp> generateQueueMap;
    std::vector<glm::vec3> generateQueueList;

    int generatedChunks = 0;
};


#endif //ZEUS_WORLD_H
