//
// Created by aurailus on 14/12/18.
//

#pragma once

#include <list>
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
#include "../../game/scene/world/graph/MeshChunk.h"

typedef unsigned int uint;

class BlockChunk {
public:
    BlockChunk() = default;
    explicit BlockChunk(const std::array<unsigned int, 4096>& blocks, const std::array<unsigned short, 4096>& biomes);
    BlockChunk(const std::array<unsigned int, 4096>& blocks, const std::array<unsigned short, 4096>& biomes, glm::ivec3 pos);

    bool shouldRender();

    bool setBlock(const glm::ivec3& pos, unsigned int ind);

    unsigned int getBlock(unsigned int ind) const;
    unsigned int getBlock(const glm::ivec3& pos) const;

    unsigned short getBiome(unsigned int ind) const;
    unsigned short getBiome(const glm::ivec3& pos) const;

    void setSunlight(unsigned int ind, unsigned char val);
    int getSunlight(unsigned int ind);
    void setBlocklight(unsigned int ind, unsigned char val);
    int getBlocklight(unsigned int ind);

    Packet serialize();
    void deserialize(Packet& packet);

    bool shouldHaveMesh = true;
    bool dirty = true;

    glm::ivec3 pos;
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