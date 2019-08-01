//
// Created by aurailus on 14/12/18.
//

#ifndef GLPROJECT_BLOCKCHUNK_H
#define GLPROJECT_BLOCKCHUNK_H

#include <glm/vec3.hpp>
#include <vector>
#include <iostream>
#include <list>
#include "../../util/Vec.h"
#include "../../util/net/Packet.h"
#include "../../util/Vec.h"
#include "../../game/scene/world/graph/MeshChunk.h"
#include "../../util/Log.h"

class BlockChunk {
public:
    BlockChunk();
    explicit BlockChunk(const std::vector<unsigned int>& blocks);
    BlockChunk(const std::vector<unsigned int>& blocks, glm::vec3 pos);

    bool shouldRender();
    bool allAdjacentsExist();

    unsigned int getBlock(unsigned int ind) const;
    unsigned int getBlock(const glm::vec3& pos) const;

    bool setBlock(const glm::vec3& pos, unsigned int ind);

    std::vector<unsigned int> rleEncode();
    void rleDecode(std::vector<unsigned int>& blocksRle, std::vector<unsigned int>& buffer);

    std::string serialize();
    bool deserialize(std::string gzip);

    glm::vec3 pos;
    bool adjacent[6] = {false, false, false, false, false, false};

    MeshChunk* meshChunk = nullptr;
    std::list<MeshChunk*>::iterator meshChunkIter;

    bool renderedEmpty = true;
private:
    std::vector<unsigned int> blocks;
    unsigned short fullBlocks = 0;
    bool empty = true;
};

#endif //GLPROJECT_BLOCKCHUNK_H