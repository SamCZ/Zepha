//
// Created by aurailus on 14/12/18.
//

#include <gzip/compress.hpp>
#include <gzip/decompress.hpp>
#include <gzip/utils.hpp>

#include "Chunk.h"

#include "../../util/net/Serializer.h"
#include "../../util/net/PacketView.h"

Chunk::Chunk(const std::vector<unsigned int>& blocks, const std::vector<unsigned short>& biomes) :
    Chunk(blocks, biomes, {0, 0, 0}) {}

Chunk::Chunk(const std::vector<unsigned int>& blocks, const std::vector<unsigned short>& biomes, glm::ivec3 pos) :
    blocks(std::move(blocks)), biomes(std::move(biomes)),
    generated(true), pos(pos) {
    recalculateRenderableBlocks();
}

bool Chunk::setBlock(unsigned int ind, unsigned int blk) {
    if (!RIE::write(ind, blk, blocks, 4096)) return false;

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

const std::vector<unsigned int> &Chunk::cGetBlocks() const {
    return blocks;
}

const std::vector<unsigned short> &Chunk::cGetBiomes() const {
    return biomes;
}

Packet Chunk::serialize() {
    Serializer s;
    s.append(pos);

    std::string temp = Serializer().append(blocks).data;
    s.append<std::string>(gzip::compress(temp.data(), temp.size()));

    temp = Serializer().append(biomes).data;
    s.append<std::string>(gzip::compress(temp.data(), temp.size()));

    std::vector<unsigned char> lights {};
    lights.resize(4096 * 4);
    for (unsigned short i = 0; i < 4096; i++) {
        lights[i * 4]     = blocklight[i].r;
        lights[i * 4 + 1] = blocklight[i].g;
        lights[i * 4 + 2] = blocklight[i].b;
        lights[i * 4 + 3] = getSunlight(i);
    }
    temp = Serializer().append(lights).data;
    s.append<std::string>(gzip::compress(temp.data(), temp.size()));

    return s.packet(PacketType::CHUNK);
}

void Chunk::deserialize(PacketView& packet) {

    pos = packet.d.read<glm::ivec3>();

    auto gzip = packet.d.read<std::string>();
    if (!gzip::is_compressed(gzip.data(), gzip.length())) throw "Invalid Blocks GZip Data.";
    gzip = gzip::decompress(gzip.data(), gzip.length());

    blocks = Deserializer(gzip).read<std::vector<unsigned int>>();
    recalculateRenderableBlocks();

    gzip = packet.d.read<std::string>();
    if (!gzip::is_compressed(gzip.data(), gzip.length())) throw "Invalid Biomes GZip Data.";
    gzip = gzip::decompress(gzip.data(), gzip.length());

    biomes = Deserializer(gzip).read<std::vector<unsigned short>>();

    gzip = packet.d.read<std::string>();
    if (!gzip::is_compressed(gzip.data(), gzip.length())) throw "Invalid Light GZip Data.";
    gzip = gzip::decompress(gzip.data(), gzip.length());

    auto lightsVec = Deserializer(gzip).read<std::vector<unsigned char>>();
    for (unsigned int i = 0; i < 4096; i++) {
        blocklight[i].r = lightsVec[i * 4];
        blocklight[i].g = lightsVec[i * 4 + 1];
        blocklight[i].b = lightsVec[i * 4 + 2];
        setSunlight(i, lightsVec[i * 4 + 3]);
    }
}

void Chunk::recalculateRenderableBlocks() {
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
