//
// Created by aurailus on 14/12/18.
//

#include "BlockChunk.h"

#include <gzip/compress.hpp>
#include <gzip/decompress.hpp>
#include <gzip/utils.hpp>

BlockChunk::BlockChunk() {
    this->blocks = new std::vector<int>();
}

BlockChunk::BlockChunk(std::vector<int>* blocks) {
    this->blocks = blocks;
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

bool BlockChunk::setBlock(glm::vec3* pos, int block) {
    unsigned int ind = ArrayTrans3D::vecToInd(pos);
    if (ind < 0 || ind >= 4096) return false;
    if (blocks->at(ind) != block) {
        (*blocks)[ind] = block;
        return true;
    }
    return false;
}

bool BlockChunk::allAdjacentsExist() {
    return adjacent[0] && adjacent[1] && adjacent[2] && adjacent[3] && adjacent[4] && adjacent[5];
}

std::vector<int>* BlockChunk::rleEncode() {
    auto rle = new std::vector<int>();

    int block = (*blocks)[0];
    int length = 1;

    for (int i = 1; i < 4096; i++) {
        if ((*blocks)[i] == block) {
            length++;
        }
        else {
            rle->push_back(block);
            rle->push_back(length);
            length = 1;
            block = (*blocks)[i];
        }
    }

    rle->push_back(block);
    rle->push_back(length);

    return rle;
}

void BlockChunk::rleDecode(std::vector<int> *blocksRle) {
    auto blocks = new std::vector<int>();
    blocks->reserve(4096);

    for (int i = 0; i < blocksRle->size() / 2; i++) {
        int block = (*blocksRle)[i*2];
        int count = (*blocksRle)[i*2 + 1];

        for (int j = 0; j < count; j++) {
            blocks->push_back(block);
            if (blocks->size() >= 4096) goto exitloop;
        }
    }

    exitloop:
    delete this->blocks;
    this->blocks = blocks;
}

std::string BlockChunk::serialize() {
    auto rle = rleEncode();
    std::string str = Packet::intVecToString(rle);
    delete rle;

    return gzip::compress(str.data(), str.size());
}

void BlockChunk::deserialize(std::string gzip) {
    if (gzip::is_compressed(gzip.data(), gzip.length())) {
        auto str = gzip::decompress(gzip.data(), gzip.length());
        auto rle = Packet::stringToIntVec(str);

        rleDecode(rle);
    }
}