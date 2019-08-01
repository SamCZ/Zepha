//
// Created by aurailus on 14/12/18.
//

#include "BlockChunk.h"
#include "../../util/Util.h"
#include "../../def/block/BlockAtlas.h"

#include <gzip/compress.hpp>
#include <gzip/decompress.hpp>
#include <gzip/utils.hpp>

BlockChunk::BlockChunk() :
    blocks(static_cast<unsigned long>(pow(TransPos::CHUNK_SIZE, 3))) {}

BlockChunk::BlockChunk(const std::vector<unsigned int>& blocks) : BlockChunk(blocks, {0, 0, 0}) {}

BlockChunk::BlockChunk(const std::vector<unsigned int>& blocks, glm::vec3 pos) :
    blocks(blocks),
    pos(pos) {

    for (int i : this->blocks) {
        if (i != 0) {
            empty = false;
            fullBlocks++;
        }
    }

    renderedEmpty = empty;
}

unsigned int BlockChunk::getBlock(const glm::vec3& pos) const {
    unsigned int ind = VecUtils::vecToInd(pos);
    if (ind >= (int)pow(TransPos::CHUNK_SIZE, 3)) return BlockAtlas::INVALID;
    return blocks[ind];
}

unsigned int BlockChunk::getBlock(unsigned int ind) const {
    if (ind >= (int)pow(TransPos::CHUNK_SIZE, 3)) return BlockAtlas::INVALID;
    return blocks[ind];
}

bool BlockChunk::setBlock(const glm::vec3& pos, unsigned int block) {
    unsigned int ind = VecUtils::vecToInd(pos);
    if (ind >= (int)pow(TransPos::CHUNK_SIZE, 3)) return false;

    if (blocks[ind] != block) {
        if (block == 0) {
            this->fullBlocks--;
            if (this->fullBlocks == 0) {
                std::cout << "Empty" << std::endl;
                this->empty = true;
                this->renderedEmpty = false;
            }
        }
        else if (blocks[ind] == 0) {
            this->fullBlocks++;
            this->empty = false;
            this->renderedEmpty = true;
        }

        blocks[ind] = block;
        return true;
    }
    return false;
}

bool BlockChunk::allAdjacentsExist() {
    return adjacent[0] && adjacent[1] && adjacent[2] && adjacent[3] && adjacent[4] && adjacent[5];
}

bool BlockChunk::shouldRender() {
    bool should = !empty || !renderedEmpty;
    renderedEmpty = true;
    return should;
}

std::vector<unsigned int> BlockChunk::rleEncode() {
    std::vector<unsigned int> rle;

    unsigned int block = blocks[0];
    unsigned int length = 1;

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

void BlockChunk::rleDecode(std::vector<unsigned int>& blocksRle, std::vector<unsigned int>& buffer) {
    int ind = 0;

    this->empty = true;

    for (int i = 0; i < blocksRle.size() / 2; i++) {
        unsigned int block = blocksRle[i*2];
        unsigned int count = blocksRle[i*2 + 1];

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
    std::vector<unsigned int> rle = rleEncode();

    std::string str;
    Serializer::encodeUIntVec(str, rle);

    return gzip::compress(str.data(), str.size());
}


//Returns a boolean indicating whether or not it was properly deserialized
bool BlockChunk::deserialize(std::string gzip) {
    if (gzip::is_compressed(gzip.data(), gzip.length())) {

        std::string str = gzip::decompress(gzip.data(), gzip.length());
        std::vector<unsigned int> rle = Serializer::decodeUIntVec(str);

        rleDecode(rle, this->blocks);
        return true;
    }
    return false;
}
