//
// Created by aurailus on 14/12/18.
//

#pragma once

#include <vector>
#include <iostream>
#include <glm/vec3.hpp>
#include "../../util/Log.h"
#include "../../util/Vec.h"
#include "../../util/Util.h"
#include "../../util/net/Packet.h"
#include "../../def/gen/BiomeAtlas.h"
#include "../../def/DefinitionAtlas.h"
#include "../../util/net/Serializer.h"
#include "../../util/net/Deserializer.h"

class BlockChunk {
public:
    BlockChunk() = default;
    explicit BlockChunk(const std::vector<unsigned int>& blocks, const std::vector<unsigned short>& biomes);
    BlockChunk(const std::vector<unsigned int>& blocks, const std::vector<unsigned short>& biomes, glm::ivec3 pos);

    void initializeEmpty();

    bool setBlock(unsigned int ind, unsigned int blk);
    inline bool setBlock(const glm::ivec3& pos, unsigned int blk);

    inline unsigned int getBlock(unsigned int ind) const;
    inline unsigned int getBlock(const glm::ivec3& pos) const;

//    bool setBiome(const glm::ivec3& pos, unsigned int ind);

    inline unsigned short getBiome(unsigned int ind) const;
    inline unsigned short getBiome(const glm::ivec3& pos) const;

    inline void setSunlight(unsigned int ind, unsigned char val);
    inline void setBlocklight(unsigned int ind, unsigned char val);

    inline int getSunlight(unsigned int ind);
    inline int getBlocklight(unsigned int ind);

    const std::vector<unsigned int>& cGetBlocks() const;
    const std::vector<unsigned short>& cGetBiomes() const;

    Packet serialize();
    void deserialize(Packet& packet);

    bool shouldHaveMesh = true;
    bool dirty = true;

    bool generated = false;

    glm::ivec3 pos;
private:
    std::vector<unsigned int> blocks {};
    std::vector<unsigned short> biomes {};
    std::array<unsigned char, 4096> lighting {};

    bool empty = true;
    bool partial = false;
    unsigned short nonAirBlocks = 0;

    friend class MapGen;
    void calcNonAirBlocks();
};

inline bool BlockChunk::setBlock(const glm::ivec3& pos, unsigned int blk) {
    if (pos.x > 15 || pos.x < 0 || pos.y > 15 || pos.y < 0 || pos.z > 15 || pos.z < 0) return false;
    return setBlock(Space::Block::index(pos), blk);
}

inline unsigned int BlockChunk::getBlock(unsigned int ind) const {
    if (ind >= 4096) return DefinitionAtlas::INVALID;
    for (unsigned int i = 0; i < blocks.size(); i += 2) {
        if (blocks[i] > ind) return blocks[i - 1];
    }
    return blocks[blocks.size() - 1];
}

inline unsigned int BlockChunk::getBlock(const glm::ivec3& pos) const {
    if (pos.x > 15 || pos.x < 0 || pos.y > 15 || pos.y < 0 || pos.z > 15 || pos.z < 0) return DefinitionAtlas::INVALID;
    return getBlock(Space::Block::index(pos));
}

inline unsigned short BlockChunk::getBiome(unsigned int ind) const {
    if (ind >= 4096) return BiomeAtlas::INVALID;
    for (unsigned int i = 0; i < biomes.size(); i += 2) {
        if (biomes[i] > ind) return biomes[i - 1];
    }
    return biomes[biomes.size() - 1];
}

inline unsigned short BlockChunk::getBiome(const glm::ivec3& pos) const {
    if (pos.x > 15 || pos.x < 0 || pos.y > 15 || pos.y < 0 || pos.z > 15 || pos.z < 0) return BiomeAtlas::INVALID;
    return getBiome(Space::Block::index(pos));
}

inline void BlockChunk::setSunlight(unsigned int ind, unsigned char val) {
    lighting[ind] = (lighting[ind] & 0xF) | (val << 4);
}

inline void BlockChunk::setBlocklight(unsigned int ind, unsigned char val) {
    lighting[ind] = (lighting[ind] & 0xF0) | val;
}

inline int BlockChunk::getSunlight(unsigned int ind) {
    return (lighting[ind] >> 4) & 0xF;
}

inline int BlockChunk::getBlocklight(unsigned int ind) {
    return (lighting[ind]) & 0xF;
}