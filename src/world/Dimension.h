//
// Created by aurailus on 04/04/19.
//

#ifndef ZEUS_DIMENSION_H
#define ZEUS_DIMENSION_H


#include <vec3.hpp>
#include <unordered_map>
#include "region/Region.h"
#include "../util/Vec.h"
#include "../game/scene/world/graph/MeshChunk.h"

class Dimension {
public:
    typedef std::unordered_map<glm::vec3, std::shared_ptr<BlockChunk>, VecUtils::compareFunc> chunk_map;

    Dimension() = default;
    explicit Dimension(glm::vec3* playerPos);

    void addBlockChunk(std::shared_ptr<BlockChunk> chunk);
    void addMeshChunk(MeshChunk* chunk);

    void update();
    void setBlock(glm::vec3 pos, int block);

    int render(Renderer &renderer);
    int getMeshChunkCount();

    std::shared_ptr<BlockChunk> getChunk(glm::vec3 pos);

    ~Dimension();
private:
    glm::vec3* playerPos = nullptr;
    std::list<MeshChunk*> meshChunks;
    chunk_map blockChunks;
};


#endif //ZEUS_DIMENSION_H
