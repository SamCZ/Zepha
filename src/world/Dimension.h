//
// Created by aurailus on 2019-11-20.
//

#pragma once

#include <memory>
#include <glm/glm.hpp>
#include <unordered_map>
#include "chunk/BlockChunk.h"

class Dimension {
public:
    Dimension() = default;

    virtual std::shared_ptr<BlockChunk> getChunk(glm::vec3 pos);
    virtual void setChunk(std::shared_ptr<BlockChunk> chunk);

    virtual unsigned int getBlock(glm::vec3 pos);
    virtual bool setBlock(glm::vec3 pos, unsigned int block);

protected:
    typedef std::unordered_map<glm::vec3, std::shared_ptr<BlockChunk>, VecUtils::compareFunc> block_chunk_map;
    block_chunk_map blockChunks;
};
