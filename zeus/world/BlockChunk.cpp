//
// Created by aurailus on 14/12/18.
//

#include <iostream>
#include "BlockChunk.h"
#include "../engine/helpers/ArrayTrans3D.h"

BlockChunk::BlockChunk(std::vector<int>* blocks) {
    this->blocks = blocks;
    this->empty = true;

    for (int i : *blocks) {
        if (i != 0) {
            empty = false;
            break;
        }
    }
}

int BlockChunk::getBlock(glm::vec3* pos) {
    unsigned int ind = ArrayTrans3D::vecToInd(pos);
    if (ind < 0 || ind >= 4096) return -1;
    return blocks->at(ind);
}

int BlockChunk::getBlock(int ind) {
    if (ind < 0 || ind >= 4096) return -1;
    return blocks->at((unsigned long)ind);
}

int BlockChunk::getBlock(int x, int y, int z) {
    unsigned int ind = ArrayTrans3D::vecToInd(x, y, z);
    if (ind < 0 || ind >= 4096) return -1;
    return blocks->at(ind);
}

bool BlockChunk::isEmpty() {
    return empty;
}