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
    // Exit early if no manipulation is needed.
    if (ind >= 4096) return false;

    const unsigned int existing = getBlock(ind);
    if (existing == blk) return false;

    // Deal with shouldHaveMesh
    if (blk == DefinitionAtlas::AIR) {
        if ((nonAirBlocks = fmax(nonAirBlocks - 1, 0)) == 0) {
            empty = true;
            shouldHaveMesh = false;
        }
    }
    else if (existing == DefinitionAtlas::AIR) {
        if (nonAirBlocks == 0) shouldHaveMesh = true;
        empty = false;
        nonAirBlocks++;
    }

    if (ind == 0) {
        if (blocks[2] == ind + 1) {
            blocks[1] = blk;
            return true;
        }
        else {
            blocks.insert(blocks.begin() + 2, 2, ind);
            blocks[2] = 1;
            blocks[3] = blocks[1];
            blocks[1] = blk;
            return true;
        }
    }

    for (unsigned int i = 0; i < blocks.size(); i += 2) {
        if (blocks[i] == ind) {
            // We found an index equating to the block we are going to be setting.
            if (blocks[i - 1] == blk) {
                // The last block strip is the same block ID as what we are setting,
                // So we should extend that strip.
                if (blocks.size() > i + 2 && blocks[i + 2] == ind + 1) {
                    // The next block is one later, meaning we can simply remove this found block
                    // To cause the next strip to cascade over its position.
                    blocks.erase(blocks.begin() + i, blocks.begin() + i + 2);
                    return true;
                }
                else {
                    // The next strip is multiple blocks over, so just add one to our found block index.
                    blocks[i] += 1;
                    return true;
                }
            }
            else {
                // The last strip is not the same block.
                if (blocks.size() > i + 2 && blocks[i + 2] == ind + 1) {
                    // There is only one of our block, so we can just update its id.
                    blocks[i + 1] = blk;
                    return true;
                }
                else {
                    // The next strip is multiple blocks over, so we need to copy the previous block to the right
                    // and then set our block into its place
                    blocks.insert(blocks.begin() + i, 2, ind);
                    blocks[i + 1] = blk;
                    blocks[i + 2] = ind + 1;
                    return true;
                }
            }
        }
        else if (blocks[i] > ind) {
            // We found a strip with an index *larger* than our ind.
            // We can assume the last strip is not our block, because the getBlock() catch would have caught that.
            if (blocks[i] == ind + 1) {
                if (blocks[i + 1] == blk) {
                    // The next block over is the same, so we can just decrement our index by one.
                    blocks[i] --;
                    return true;
                }
                else {
                    // There is only one of our block to be placed, directly before our current strip
                    blocks.insert(blocks.begin() + i, 2, ind);
                    blocks[i + 1] = blk;
                    return true;
                }
            }
            else {
                // The next strip is multiple blocks over, so we need to insert both our block
                // *and* the previous strip's block after
                blocks.insert(blocks.begin() + i, 4, ind);
                blocks[i + 1] = blk;
                blocks[i + 2] = ind + 1;
                blocks[i + 3] = blocks[i - 1];
                return true;
            }
        }
    }
    // Escaped the for loop, meaning there's no index greater than ours.
    // We will insert our index at the end of the array, and insert the previous block after
    // if we're not at the end of the chunk.
    blocks.push_back(ind);
    blocks.push_back(blk);

    if (ind >= 4095) return true; // Don't add the reset if at the end of the chunk.

    blocks.push_back(ind + 1);
    blocks.push_back(blocks[blocks.size() - 4]);
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
        unsigned int lInd = (i == 0 ? 0 : blocks[i - 2]);
        unsigned int blk  = blocks[i + 1];

        if (blk > DefinitionAtlas::AIR) {
            empty = false;
            nonAirBlocks += cInd - lInd;
        }
    }

    shouldHaveMesh = !empty;
}

void BlockChunk::initializeEmpty() {
    blocks = {0, 0};
    biomes = {0, 0};
}
