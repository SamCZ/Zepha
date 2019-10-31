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
    explicit BlockChunk(const std::array<uint, 4096>& blocks);
    BlockChunk(const std::array<uint, 4096>& blocks, glm::vec3 pos);

    bool shouldRender();

    unsigned int getBlock(unsigned int ind) const;
    unsigned int getBlock(const glm::vec3& pos) const;

    bool setBlock(const glm::vec3& pos, unsigned int ind);

    std::vector<unsigned int> rleEncode();
    void rleDecode(std::vector<unsigned int>& blocksRle, std::array<uint, 4096>& buffer);

    std::string serialize();
    bool deserialize(std::string gzip);

    glm::vec3 pos;

    bool renderedEmpty = true;
    bool dirty = true;
private:
    std::array<uint, 4096> blocks {};
    unsigned short fullBlocks = 0;

    bool empty = true;
};
