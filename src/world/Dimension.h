//
// Created by aurailus on 04/04/19.
//

#pragma once

#include <unordered_map>
#include <glm/vec3.hpp>
#include "region/Region.h"
#include "../util/Vec.h"
#include "../game/scene/world/graph/MeshChunk.h"
#include "../game/scene/world/graph/ChunkRenderRef.h"
//#include "../game/scene/world/MeshGenStream.h"

class Dimension {
public:

    void setChunk(std::shared_ptr<BlockChunk> chunk);
    void setMeshChunk(std::shared_ptr<MeshChunk> chunk);
    void removeMeshChunk(const glm::vec3& pos);

    void update(glm::vec3 playerPos);

    void setBlock(glm::vec3 pos, unsigned int block);
    unsigned int getBlock(glm::vec3 pos);

    int render(Renderer &renderer);
    int getMeshChunkCount();

    std::shared_ptr<BlockChunk> getChunk(glm::vec3 pos);
private:
//    void attemptMeshChunk(const sptr<BlockChunk>& chunk);
//    bool getAdjacentExists(glm::vec3 pos);

    glm::vec3 playerPos {};

//    MeshGenStream meshGenStream;
    std::unordered_map<glm::vec3, ChunkRenderRef, VecUtils::compareFunc> renderRefs;
    std::list<std::shared_ptr<ChunkRenderElem>> renderElems;

    typedef std::unordered_map<glm::vec3, std::shared_ptr<BlockChunk>, VecUtils::compareFunc> block_chunk_map;
    block_chunk_map blockChunks;
};

