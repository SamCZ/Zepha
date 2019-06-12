//
// Created by aurailus on 14/12/18.
//

#include "BlockChunk.h"

#include <gzip/compress.hpp>
#include <gzip/decompress.hpp>
#include <gzip/utils.hpp>

BlockChunk::BlockChunk() {
    this->blocks = std::vector<int>((unsigned long)pow(TransPos::CHUNK_SIZE, 3));
    this->empty = true;
}

BlockChunk::BlockChunk(std::vector<int> blocks) {
    this->empty = true;
    for (int i : blocks) {
        if (i != 0) this->empty = false;
    }

    this->blocks = std::move(blocks);
}

BlockChunk::BlockChunk(std::vector<int> blocks, glm::vec3 pos) {
    this->empty = true;
    for (int i : blocks) {
        if (i != 0) this->empty = false;
    }

    this->blocks = std::move(blocks);
    this->pos = pos;
}

int BlockChunk::getBlock(glm::vec3* pos) {
    unsigned int ind = VecUtils::vecToInd(pos);
    if (ind < 0 || ind >= (int)pow(TransPos::CHUNK_SIZE, 3)) return -1;
    return blocks[ind];
}

int BlockChunk::getBlock(int ind) {
    if (ind < 0 || ind >= (int)pow(TransPos::CHUNK_SIZE, 3)) return -1;
    return blocks[ind];
}

int BlockChunk::getBlock(int x, int y, int z) {
    unsigned int ind = VecUtils::vecToInd(x, y, z);
    if (ind < 0 || ind >= (int)pow(TransPos::CHUNK_SIZE, 3)) return -1;
    return blocks[ind];
}

bool BlockChunk::setBlock(glm::vec3* pos, int block) {

    //TODO: Update emptiness

    unsigned int ind = VecUtils::vecToInd(pos);
    if (ind < 0 || ind >= (int)pow(TransPos::CHUNK_SIZE, 3)) return false;
    if (blocks[ind] != block) {
        blocks[ind] = block;
        return true;
    }
    return false;
}

bool BlockChunk::allAdjacentsExist() {
    return adjacent[0] && adjacent[1] && adjacent[2] && adjacent[3] && adjacent[4] && adjacent[5];
}

bool BlockChunk::isEmpty() {
    return empty;
}

std::vector<int> BlockChunk::rleEncode() {
    std::vector<int> rle;

    int block = blocks[0];
    int length = 1;

    for (int i = 1; i < (int)pow(TransPos::CHUNK_SIZE, 3); i++) {
        if (blocks[i] == block) {
            length++;
        }
        else {
            rle.push_back(block);
            rle.push_back(length);
            length = 1;
            block = blocks[i];
        }
    }

    rle.push_back(block);
    rle.push_back(length);

    return rle;
}

void BlockChunk::rleDecode(std::vector<int>& blocksRle, std::vector<int>& buffer) {
    int ind = 0;

    this->empty = true;

    for (int i = 0; i < blocksRle.size() / 2; i++) {
        int block = blocksRle[i*2];
        int count = blocksRle[i*2 + 1];

        for (int j = 0; j < count; j++) {
            buffer[ind++] = (block);

            if (block != 0) {
                this->empty = false;
            }

            if (ind >= (int)pow(TransPos::CHUNK_SIZE, 3)) return;
        }
    }
}

std::string BlockChunk::serialize() {
    auto rle = rleEncode();

    std::string str;
    Serializer::encodeIntVec(str, rle);

    return gzip::compress(str.data(), str.size());
}


//Returns a boolean indicating whether or not it was properly deserialized
bool BlockChunk::deserialize(std::string gzip) {
    if (gzip::is_compressed(gzip.data(), gzip.length())) {

        auto str = gzip::decompress(gzip.data(), gzip.length());
        auto rle = Serializer::decodeIntVec(str);

        rleDecode(rle, this->blocks);
        return true;
    }
    return false;
}

std::vector<int> BlockChunk::deserializeToVec(std::string gzip) {
    if (gzip::is_compressed(gzip.data(), gzip.length())) {

        auto str = gzip::decompress(gzip.data(), gzip.length());
        auto rle = Serializer::decodeIntVec(str);

        std::vector<int> vals((unsigned long)pow(TransPos::CHUNK_SIZE, 3));
        int ind = 0;

        for (int i = 0; i < rle.size() / 2; i++) {
            int block = rle[i*2];
            int count = rle[i*2 + 1];

            for (int j = 0; j < count; j++) {
                vals[ind++] = (block);

                if (ind >= (int)pow(TransPos::CHUNK_SIZE, 3)) break;
            }
        }

        return std::move(vals);
    }

    std::cout << Log::err << "Badly coded packet!" << Log::endl;
    return {};
}
