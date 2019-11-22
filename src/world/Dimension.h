//
// Created by aurailus on 2019-11-20.
//

#pragma once

#include <memory>
#include <glm/glm.hpp>
#include <unordered_map>
#include "chunk/BlockChunk.h"
#include "region/Region.h"

class Dimension {
public:
    Dimension() = default;

    std::shared_ptr<Region> getRegion(glm::vec3 regionPosition);
    std::shared_ptr<MapBlock> getMapBlock(glm::vec3 mapBlockPosition);
    std::shared_ptr<BlockChunk> getChunk(glm::vec3 chunkPosition);

    virtual void setChunk(std::shared_ptr<BlockChunk> chunk);

    unsigned int getBlock(glm::vec3 pos);
    virtual bool setBlock(glm::vec3 pos, unsigned int block);

protected:
    typedef std::unordered_map<glm::vec3, std::shared_ptr<Region>, Vec::compareFunc> block_region_map;
    block_region_map regions;

private:
    inline std::shared_ptr<Region> getOrCreateRegion(glm::vec3 pos);
    inline std::shared_ptr<MapBlock> getOrCreateMapBlock(glm::vec3 mapBlockPosition);
};
