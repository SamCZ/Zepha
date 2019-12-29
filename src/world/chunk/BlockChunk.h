//
// Created by aurailus on 14/12/18.
//

#pragma once

#include <glm/vec3.hpp>
#include <vector>
#include <iostream>
#include <list>
#include "../../util/Vec.h"
#include "../../util/net/Packet.h"
#include "../../game/scene/world/graph/MeshChunk.h"
#include "../../util/Log.h"

typedef unsigned int uint;

class BlockChunk {
public:
    BlockChunk() = default;
    explicit BlockChunk(const std::array<unsigned int, 4096>& blocks, const std::array<unsigned short, 4096>& biomes);
    BlockChunk(const std::array<unsigned int, 4096>& blocks, const std::array<unsigned short, 4096>& biomes, glm::vec3 pos);

    bool shouldRender();

    bool setBlock(const glm::vec3& pos, unsigned int ind);

    unsigned int getBlock(unsigned int ind) const;
    unsigned int getBlock(const glm::vec3& pos) const;

    unsigned short getBiome(unsigned int ind) const;
    unsigned short getBiome(const glm::vec3& pos) const;

    void setSunlight(unsigned int ind, unsigned char val);
    int getSunlight(unsigned int ind);
    void setBlocklight(unsigned int ind, unsigned char val);
    int getBlocklight(unsigned int ind);

    std::string serialize();
    void deserialize(const char* packet);

    bool renderedEmpty = true;
    bool dirty = true;

    glm::vec3 pos;
private:
    std::array<unsigned int, 4096> blocks {};
    std::array<unsigned short, 4096> biomes {};
    std::array<unsigned char, 4096> lighting {};

    unsigned short fullBlocks = 0;
    bool empty = true;

    //Exclusive Access for MapGen to speed up chunk creation
    void mgRegenEmpty();
    friend class MapGen;
};

inline void BlockChunk::setSunlight(unsigned int ind, unsigned char val) {
    lighting[ind] = (lighting[ind] & 0xF) | (val << 4);
}

inline int BlockChunk::getSunlight(unsigned int ind) {
    return (lighting[ind] >> 4) & 0xF;
}

inline void BlockChunk::setBlocklight(unsigned int ind, unsigned char val) {
    lighting[ind] = (lighting[ind] & 0xF0) | val;
}

inline int BlockChunk::getBlocklight(unsigned int ind) {
    return (lighting[ind]) & 0xF;
}