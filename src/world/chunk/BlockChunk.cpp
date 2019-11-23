//
// Created by aurailus on 14/12/18.
//

#include "BlockChunk.h"
#include "../../util/Util.h"
#include "../../def/DefinitionAtlas.h"
#include "../../def/gen/BiomeAtlas.h"

#include <gzip/compress.hpp>
#include <gzip/decompress.hpp>
#include <gzip/utils.hpp>

BlockChunk::BlockChunk(const std::array<unsigned int, 4096>& blocks, const std::array<unsigned short, 4096>& biomes) :
    BlockChunk(blocks, biomes, {0, 0, 0}) {}

BlockChunk::BlockChunk(const std::array<unsigned int, 4096>& blocks, const std::array<unsigned short, 4096>& biomes, glm::vec3 pos) :
    blocks(std::move(blocks)),
    biomes(std::move(biomes)),
    pos(pos) {

    for (unsigned int block : this->blocks) {
        if (block > DefinitionAtlas::AIR) {
            empty = false;
            fullBlocks++;
        }
    }

    renderedEmpty = empty;
}

unsigned int BlockChunk::getBlock(const glm::vec3& pos) const {
    unsigned int ind = Space::Block::index(pos);
    if (ind >= 4096) return DefinitionAtlas::INVALID;
    return blocks[ind];
}

unsigned int BlockChunk::getBlock(unsigned int ind) const {
    if (ind >= 4096) return DefinitionAtlas::INVALID;
    return blocks[ind];
}

unsigned short BlockChunk::getBiome(const glm::vec3& pos) const {
    unsigned int ind = Space::Block::index(pos);
    if (ind >= 4096) return BiomeAtlas::INVALID;
    return biomes[ind];
}

unsigned short BlockChunk::getBiome(unsigned int ind) const {
    if (ind >= 4096) return BiomeAtlas::INVALID;
    return biomes[ind];
}

bool BlockChunk::setBlock(const glm::vec3& pos, unsigned int block) {
    unsigned int ind = Space::Block::index(pos);
    if (ind >= (int)pow(16, 3)) return false;

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

bool BlockChunk::shouldRender() {
    bool should = !empty || !renderedEmpty;
    renderedEmpty = true;
    return should;
}

std::string BlockChunk::serialize() {
    std::string blocksGzip {};
    std::string biomesGzip {};

    {
        unsigned int curr = blocks[0];
        unsigned int length = 1;

        std::vector<unsigned int> blocksRle;

        for (int i = 1; i < 4096; i++) {
            if (blocks[i] == curr) length++;
            else {
                blocksRle.push_back(curr);
                blocksRle.push_back(length);
                length = 1;
                curr = blocks[i];
            }
        }
        blocksRle.push_back(curr);
        blocksRle.push_back(length);

        std::string temp;
        Serializer::encodeUIntVec(temp, blocksRle);
        blocksGzip = gzip::compress(temp.data(), temp.size());
    }
    {
        unsigned int curr = biomes[0];
        unsigned short length = 1;

        std::vector<unsigned short> biomesRle;

        for (int i = 1; i < 4096; i++) {
            if (biomes[i] == curr) length++;
            else {
                biomesRle.push_back(curr);
                biomesRle.push_back(length);
                length = 1;
                curr = biomes[i];
            }
        }
        biomesRle.push_back(curr);
        biomesRle.push_back(length);

        std::string temp = "";
        Serializer::encodeUShortVec(temp, biomesRle);
        biomesGzip = gzip::compress(temp.data(), temp.size());
    }

    std::string data = "";
    Serializer::encodeString(data, blocksGzip);
    Serializer::encodeString(data, biomesGzip);
    return data;
}

void BlockChunk::deserialize(const char* packet) {
    size_t biomeOffset;

    {
        std::string str;

        std::string blocksGzip = Serializer::decodeString(packet);
        biomeOffset = blocksGzip.length() + 4;

        if (!gzip::is_compressed(blocksGzip.data(), blocksGzip.length())) throw "Invalid Blocks GZip Data.";
        blocksGzip = gzip::decompress(blocksGzip.data(), blocksGzip.length());
        std::vector<unsigned int> blocksRle = Serializer::decodeUIntVec(blocksGzip);

        int ind = 0;
        this->empty = true;

        for (int i = 0; i < blocksRle.size() / 2; i++) {
            unsigned int block = blocksRle[i*2];
            unsigned int count = blocksRle[i*2 + 1];

            for (int j = 0; j < count; j++) {
                blocks[ind++] = block;
                if (block != 0) this->empty = false;
                if (ind >= 4096) goto stop1;
            }
        }
        stop1: {};
    }
    {
        std::string str;

        std::string biomesGzip = Serializer::decodeString(&packet[biomeOffset]);
        if (!gzip::is_compressed(biomesGzip.data(), biomesGzip.length())) throw "Invalid Biomes GZip Data.";
        biomesGzip = gzip::decompress(biomesGzip.data(), biomesGzip.length());
        std::vector<unsigned short> biomesRle = Serializer::decodeUShortVec(biomesGzip);

        int ind = 0;

        for (int i = 0; i < biomesRle.size() / 2; i++) {
            unsigned int biome = biomesRle[i*2];
            unsigned int count = biomesRle[i*2 + 1];

            for (int j = 0; j < count; j++) {
                biomes[ind++] = biome;
                if (ind >= 4096) goto stop2;
            }
        }
        stop2: {};
    }
}

void BlockChunk::mgRegenEmpty() {
    fullBlocks = 0;
    empty = true;

    for (unsigned int block : this->blocks) {
        if (block > DefinitionAtlas::AIR) {
            empty = false;
            fullBlocks++;
        }
    }

    renderedEmpty = empty;
}
