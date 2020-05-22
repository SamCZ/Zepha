//
// Created by aurailus on 2019-11-20.
//

#pragma once

#include <queue>
#include <array>
#include <memory>
#include <unordered_set>

#include "../util/Vec.h"
#include "region/Region.h"

class Dimension {
public:
    Dimension(DefinitionAtlas& defs);

    std::shared_ptr<Region> getRegion(glm::ivec3 regionPosition);
    void removeRegion(glm::ivec3 pos);

    std::shared_ptr<MapBlock> getMapBlock(glm::ivec3 mapBlockPosition);
    virtual void removeMapBlock(glm::ivec3 pos);

    std::shared_ptr<BlockChunk> getChunk(glm::ivec3 chunkPosition);
    virtual void setChunk(std::shared_ptr<BlockChunk> chunk);
    virtual void removeChunk(glm::ivec3 pos);

    unsigned int getBlock(glm::ivec3 pos);
    virtual bool setBlock(glm::ivec3 pos, unsigned int block);

    void createSunlight(glm::ivec3 pos);
    void propogateLight();
protected:
    typedef std::unordered_map<glm::ivec3, std::shared_ptr<Region>, Vec::ivec3> block_region_map;
    block_region_map regions;
    DefinitionAtlas& defs;

    virtual std::unordered_set<glm::ivec3, Vec::ivec3> propogateAddNodes();
    virtual std::unordered_set<glm::ivec3, Vec::ivec3> propogateRemoveNodes();
private:
    inline std::shared_ptr<Region> getOrCreateRegion(glm::ivec3 pos);
    inline std::shared_ptr<MapBlock> getOrCreateMapBlock(glm::ivec3 mapBlockPosition);

    static inline bool containsWorldPos(BlockChunk* chunk, glm::ivec3 pos);
    inline glm::ivec4 getLight(glm::ivec3 worldPos, BlockChunk* chunk = nullptr);

    inline void addLight(glm::ivec3 pos, glm::ivec3 light);
    inline void removeLight(glm::ivec3 pos);

    struct LightAddNode {
        LightAddNode(unsigned short index, BlockChunk* chunk) : index(index), chunk(chunk) {};
        unsigned short index;
        BlockChunk* chunk;
    };

    struct LightRemoveNode {
        LightRemoveNode(unsigned short index, unsigned short value, BlockChunk* chunk) : index(index), value(value), chunk(chunk) {};
        unsigned short index;
        unsigned short value;
        BlockChunk* chunk;
    };

    static constexpr unsigned char SUNLIGHT_CHANNEL = 3;
    std::array<std::queue<LightAddNode>, 4> lightAddQueue;
    std::array<std::queue<LightRemoveNode>, 4> lightRemoveQueue;
};
