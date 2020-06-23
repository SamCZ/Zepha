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
    std::vector<unsigned short> blockLight = std::vector<unsigned short>(4096);
    std::vector<unsigned char> sunLight = std::vector<unsigned char>(2048);

    for (unsigned short i = 0; i < 4096; i++) {
        blocklight_union bl;
        bl.b = this->blockLight[i];
        blockLight[i] = bl.sh;
    }

    for (unsigned short i = 0; i < 2048; i++) {
        sunlight_union sl;
        sl.s = this->sunLight[i];
        sunLight[i] = sl.ch;
    }

    Serializer s;
    std::string temp = Serializer().append(pos).append(blocks).append(biomes).append(blockLight).append(sunLight).data;
    s.append<std::string>(gzip::compress(temp.data(), temp.size()));

    std::cout << s.data.length() << std::endl;

    return s.packet(PacketType::CHUNK);
}

void Chunk::deserialize(PacketView& packet) {
    std::string gzipped = packet.d.read<std::string>();
    if (!gzip::is_compressed(gzipped.data(), gzipped.length())) throw "Invalid Blocks GZip Data.";

    std::vector<unsigned char> sunLight {};
    std::vector<unsigned short> blockLight {};
    Deserializer(gzip::decompress(gzipped.data(), gzipped.length()))
        .read<glm::ivec3>(pos)
        .read<std::vector<unsigned int>>(blocks)
        .read<std::vector<unsigned short>>(biomes)
        .read<std::vector<unsigned short>>(blockLight)
        .read<std::vector<unsigned char>>(sunLight);

    for (unsigned short i = 0; i < 4096; i++) {
        blocklight_union bl;
        bl.sh = blockLight[i];
        this->blockLight[i] = bl.b;
    }

    for (unsigned short i = 0; i < 2048; i++) {
        sunlight_union sl;
        sl.ch = sunLight[i];
        this->sunLight[i] = sl.s;
    }

    recalculateRenderableBlocks();
}

void Chunk::recalculateRenderableBlocks() {
    shouldHaveMesh = false;
    nonAirBlocks = 0;

    for (unsigned int i = 0; i < blocks.size(); i += 2) {
        unsigned int nInd = (i == blocks.size() - 2 ? 4095 : blocks[i + 2]);
        unsigned int cInd = blocks[i];

        if (blocks[i + 1] > DefinitionAtlas::AIR) {
            nonAirBlocks += nInd - cInd;
            shouldHaveMesh = true;
        }
    }
}
