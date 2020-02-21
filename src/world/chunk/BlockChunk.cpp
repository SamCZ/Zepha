//
// Created by aurailus on 14/12/18.
//

#include "BlockChunk.h"

#include <gzip/compress.hpp>
#include <gzip/decompress.hpp>
#include <gzip/utils.hpp>

BlockChunk::BlockChunk(const std::vector<unsigned int>& blocks, const std::vector<unsigned short>& biomes) :
    BlockChunk(blocks, biomes, {0, 0, 0}) {}

BlockChunk::BlockChunk(const std::vector<unsigned int>& blocks, const std::vector<unsigned short>& biomes, glm::ivec3 pos) :
    blocks(std::move(blocks)),
    biomes(std::move(biomes)),
    pos(pos),
    generated(true) {
    calcNonAirBlocks();
}

bool BlockChunk::setBlock(unsigned int ind, unsigned int blk) {
    if (!RIE::write(ind, blk, blocks, 4096)) return false;

    // Mesh emptiness manipulation
    if (blk == DefinitionAtlas::AIR) {
        if ((nonAirBlocks = fmax(nonAirBlocks - 1, 0)) == 0) {
            empty = true;
            shouldHaveMesh = false;
        }
    }
    else if (getBlock(ind) == DefinitionAtlas::AIR) {
        shouldHaveMesh = true;
        empty = false;
        nonAirBlocks++;
    }

    return true;
}

const std::vector<unsigned int> &BlockChunk::cGetBlocks() const {
    return blocks;
}

const std::vector<unsigned short> &BlockChunk::cGetBiomes() const {
    return biomes;
}

Packet BlockChunk::serialize() {
    Serializer s;
    s.append(pos);

    std::string temp = Serializer().append(blocks).data;
    s.append<std::string>(gzip::compress(temp.data(), temp.size()));

    temp = Serializer().append(biomes).data;
    s.append<std::string>(gzip::compress(temp.data(), temp.size()));

    return s.packet(PacketType::CHUNK);
}

void BlockChunk::deserialize(Packet& packet) {
    Deserializer d(packet.data);

    pos = d.read<glm::ivec3>();

    auto gzip = d.read<std::string>();
    if (!gzip::is_compressed(gzip.data(), gzip.length())) throw "Invalid Blocks GZip Data.";
    gzip = gzip::decompress(gzip.data(), gzip.length());

    blocks = Deserializer(gzip).read<std::vector<unsigned int>>();
    calcNonAirBlocks();

    gzip = d.read<std::string>();
    if (!gzip::is_compressed(gzip.data(), gzip.length())) throw "Invalid Biomes GZip Data.";
    gzip = gzip::decompress(gzip.data(), gzip.length());

    biomes = Deserializer(gzip).read<std::vector<unsigned short>>();
}

void BlockChunk::calcNonAirBlocks() {
    nonAirBlocks = 0;
    empty = true;

    for (unsigned int i = 0; i < blocks.size(); i += 2) {
        unsigned int cInd = blocks[i];
        unsigned int nInd = (i == blocks.size() - 2 ? 4095 : blocks[i + 2]);
        unsigned int blk  = blocks[i + 1];

        if (blk > DefinitionAtlas::AIR) {
            empty = false;
            nonAirBlocks += nInd - cInd;
        }
    }

    shouldHaveMesh = !empty;
}

void BlockChunk::initializeEmpty() {
    blocks = {0, 0};
    biomes = {0, 0};
}
