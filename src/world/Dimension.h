//
// DimensionBase subclass that handles block light and sunlight.
// This class was split from DimensionBase to help differentiate
// lighting functions from basic world manipulation.
// Created by aurailus on 2020-05-26.
//

#pragma once

#include <unordered_set>
#include <queue>

#include "DimensionBase.h"

class Dimension : public DimensionBase {
public:
    typedef std::unordered_set<glm::ivec3, Vec::ivec3> relitChunks;

    Dimension(DefinitionAtlas& defs) : DimensionBase(defs) {}

    // Override setBlock to update lighting.
    bool setBlock(glm::ivec3 pos, unsigned int block) override;

    // Calculate light propogation around MapBlock edges,
    // Called after a new mapblock is inserted into the dimension.
    relitChunks calculateEdgeLight(glm::ivec3 mbPos);

protected:
    virtual relitChunks propogateAddNodes();
    virtual relitChunks propogateRemoveNodes();

private:
    // Lighting functions

    static inline bool containsWorldPos(BlockChunk* chunk, glm::ivec3 pos);
    inline glm::ivec4 getLight(glm::ivec3 worldPos, BlockChunk* chunk = nullptr);

    void calculateHorizontalEdge(std::shared_ptr<BlockChunk> a, std::shared_ptr<BlockChunk> b);
    void calculateVerticalEdge(std::shared_ptr<BlockChunk> above, std::shared_ptr<BlockChunk> below);

    inline void addBlockLight(glm::ivec3 pos, glm::ivec3 light);
    inline void removeBlockLight(glm::ivec3 pos);

    inline void reflowLight(glm::ivec3 pos);
    inline void removeSunlight(glm::ivec3 pos);
    inline void setSunlight(glm::ivec3 pos, unsigned char level);

    struct LightAddNode {
        LightAddNode(unsigned short index, BlockChunk* chunk) : index(index), chunk(chunk) {};
        unsigned short index; BlockChunk* chunk;
    };
    struct LightRemoveNode {
        LightRemoveNode(unsigned short index, unsigned short value, BlockChunk* chunk) : index(index), value(value), chunk(chunk) {};
        unsigned short index, value; BlockChunk* chunk;
    };

    static constexpr unsigned char SUNLIGHT_CHANNEL = 3;
    std::array<std::queue<LightAddNode>, 4> lightAddQueue;
    std::array<std::queue<LightRemoveNode>, 4> lightRemoveQueue;
};
