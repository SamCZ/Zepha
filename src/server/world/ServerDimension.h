//
// Created by aurailus on 01/10/19.
//

#pragma once

#include <unordered_map>
#include <glm/vec3.hpp>
#include "../../world/chunk/BlockChunk.h"

class ServerDimension {
public:
    ServerDimension() = default;

    void setChunk(std::shared_ptr<BlockChunk> chunk);
    std::shared_ptr<BlockChunk> getChunk(glm::vec3 pos);

    void setBlock(glm::vec3 pos, uint block);
    uint getBlock(glm::vec3 pos);
private:
    typedef std::unordered_map<glm::vec3, std::shared_ptr<BlockChunk>, VecUtils::compareFunc> block_chunk_map;
    block_chunk_map blockChunks;
};

