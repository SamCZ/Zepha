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
#include "../../util/Vec.h"
#include "../../game/scene/world/graph/MeshChunk.h"
#include "../../util/Log.h"

typedef unsigned int uint;

class BlockChunk {
public:
    BlockChunk() = default;
    explicit BlockChunk(const std::array<unsigned int, 4096>& blocks, const std::array<unsigned short, 4096>& biomes);
    BlockChunk(const std::array<unsigned int, 4096>& blocks, const std::array<unsigned short, 4096>& biomes, glm::vec3 pos);

    bool shouldRender();

    unsigned int getBlock(unsigned int ind) const;
    unsigned int getBlock(const glm::vec3& pos) const;
    unsigned short getBiome(unsigned int ind) const;
    unsigned short getBiome(const glm::vec3& pos) const;

    bool setBlock(const glm::vec3& pos, unsigned int ind);

    std::string serialize();
    void deserialize(const char* packet);

    glm::vec3 pos;

    bool renderedEmpty = true;
    bool dirty = true;
private:
    std::array<unsigned int, 4096> blocks {};
    std::array<unsigned short, 4096> biomes {};
    unsigned short fullBlocks = 0;

    bool empty = true;

    //Exclusive Access for MapGen to speed up chunk creation
    void mgRegenEmpty();
    friend class MapGen;
};
