//
// Created by aurailus on 14/12/18.
//

#include "BlockChunk.h"

#include <gzip/compress.hpp>
#include <gzip/decompress.hpp>
#include <gzip/utils.hpp>

BlockChunk::BlockChunk(const std::array<unsigned int, 4096>& blocks, const std::array<unsigned short, 4096>& biomes) :
    BlockChunk(blocks, biomes, {0, 0, 0}) {}

BlockChunk::BlockChunk(const std::array<unsigned int, 4096>& blocks, const std::array<unsigned short, 4096>& biomes, glm::ivec3 pos) :
    blocks(std::move(blocks)),
    biomes(std::move(biomes)),
    pos(pos) {

    for (unsigned int block : this->blocks) {
        if (block != DefinitionAtlas::AIR) {
            empty = false;
            fullBlocks++;
        }
    }

    renderedEmpty = empty;
}

bool BlockChunk::shouldRender() {
    bool should = !empty || !renderedEmpty;
    renderedEmpty = true;
    return should;
}

bool BlockChunk::setBlock(const glm::ivec3& pos, unsigned int block) {
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

unsigned int BlockChunk::getBlock(unsigned int ind) const {
    if (ind >= 4096) return DefinitionAtlas::INVALID;
    return blocks[ind];
}

unsigned int BlockChunk::getBlock(const glm::ivec3& pos) const {
    unsigned int ind = Space::Block::index(pos);
    if (ind >= 4096) return DefinitionAtlas::INVALID;
    return blocks[ind];
}

unsigned short BlockChunk::getBiome(unsigned int ind) const {
    if (ind >= 4096) return BiomeAtlas::INVALID;
    return biomes[ind];
}

unsigned short BlockChunk::getBiome(const glm::ivec3& pos) const {
    unsigned int ind = Space::Block::index(pos);
    if (ind >= 4096) return BiomeAtlas::INVALID;
    return biomes[ind];
}

Packet BlockChunk::serialize() {
    Serializer s;
    s.append(pos);

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

        std::string temp = Serializer().append(blocksRle).data;
        s.append<std::string>(gzip::compress(temp.data(), temp.size()));
    } {
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

        std::string temp = Serializer().append(biomesRle).data;
        s.append<std::string>(gzip::compress(temp.data(), temp.size()));
    }

    Packet p(PacketType::CHUNK);
    p.data = s.data;
    return p;
}

void BlockChunk::deserialize(Packet& packet) {
    Deserializer d(packet.data);

    pos = d.read<glm::ivec3>();

    {
        auto gzip = d.read<std::string>();
        if (!gzip::is_compressed(gzip.data(), gzip.length())) throw "Invalid Blocks GZip Data.";
        gzip = gzip::decompress(gzip.data(), gzip.length());

        std::vector<unsigned int> rle = Deserializer(gzip).read<std::vector<unsigned int>>();

        this->empty = true;

        int ind = 0;
        for (int i = 0; i < rle.size() / 2; i++) {
            unsigned int block = rle[i * 2];
            unsigned int count = rle[i * 2 + 1];

            for (int j = 0; j < count; j++) {
                blocks[ind++] = block;
                if (block != 0) this->empty = false;
                if (ind >= 4096) goto biomes;
            }
        }
    }
    biomes: {}
    {
        auto gzip = d.read<std::string>();
        if (!gzip::is_compressed(gzip.data(), gzip.length())) throw "Invalid Biomes GZip Data.";
        gzip = gzip::decompress(gzip.data(), gzip.length());

        std::vector<unsigned short> rle = Deserializer(gzip).read<std::vector<unsigned short>>();

        int ind = 0;
        for (int i = 0; i < rle.size() / 2; i++) {
            unsigned int biome = rle[i * 2];
            unsigned int count = rle[i * 2 + 1];

            for (int j = 0; j < count; j++) {
                biomes[ind++] = biome;
                if (ind >= 4096) goto end;
            }
        }
    }
    end: {}
}

void BlockChunk::mgRegenEmpty() {
    fullBlocks = 0;
    empty = true;

    for (unsigned int block : this->blocks) {
        if (block != DefinitionAtlas::AIR) {
            empty = false;
            fullBlocks++;
        }
    }

    renderedEmpty = empty;
}