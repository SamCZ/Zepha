//
// The base superclass for both dimensions. Handles chunk storage and manipulation.
// Does not include lighting mechanics, which are contained in subclass Dimension.
// Created by aurailus on 2019-11-20.
//

#pragma once

#include <memory>

#include "../util/Vec.h"
#include "chunk/Region.h"

class DimensionBase {
public:
    DimensionBase(DefinitionAtlas& defs);

    std::shared_ptr<Region> getRegion(glm::ivec3 regionPosition);
    void removeRegion(glm::ivec3 pos);

    std::shared_ptr<MapBlock> getMapBlock(glm::ivec3 mapBlockPosition);
    virtual void removeMapBlock(glm::ivec3 pos);

    bool mapBlockGenerated(glm::ivec3 mapBlockPosition);

    std::shared_ptr<Chunk> getChunk(glm::ivec3 chunkPosition);
    virtual void setChunk(std::shared_ptr<Chunk> chunk);
    virtual void removeChunk(glm::ivec3 pos);

    unsigned int getBlock(glm::ivec3 pos);
    virtual bool setBlock(glm::ivec3 pos, unsigned int block);
protected:

    typedef std::unordered_map<glm::ivec3, std::shared_ptr<Region>, Vec::ivec3> block_region_map;
    block_region_map regions;
    DefinitionAtlas& defs;
private:

    inline std::shared_ptr<Region> getOrCreateRegion(glm::ivec3 pos);
    inline std::shared_ptr<MapBlock> getOrCreateMapBlock(glm::ivec3 mapBlockPosition);
};
