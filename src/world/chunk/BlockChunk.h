//
// Created by aurailus on 14/12/18.
//

#pragma once

#include <vector>
#include <glm/vec3.hpp>

#include "../../util/RIE.h"
#include "../../util/Space.h"
#include "../../util/net/Packet.h"
#include "../../def/gen/BiomeAtlas.h"
#include "../../def/DefinitionAtlas.h"
#include "../../util/net/PacketView.h"

class BlockChunk {
public:
    BlockChunk() = default;
    explicit BlockChunk(const std::vector<unsigned int>& blocks, const std::vector<unsigned short>& biomes);
    BlockChunk(const std::vector<unsigned int>& blocks, const std::vector<unsigned short>& biomes, glm::ivec3 pos);

    void initializeEmpty();

    // Blocks

    inline unsigned int getBlock(unsigned int ind) const;
    inline unsigned int getBlock(const glm::ivec3& pos) const;

    bool setBlock(unsigned int ind, unsigned int blk);
    inline bool setBlock(const glm::ivec3& pos, unsigned int blk);

    const std::vector<unsigned int>& cGetBlocks() const;

    // Biomes

    inline unsigned short getBiome(unsigned int ind) const;
    inline unsigned short getBiome(const glm::ivec3& pos) const;

    inline bool setBiome(unsigned int ind, unsigned short bio);
    inline bool setBiome(const glm::ivec3& pos, unsigned short bio);
    const std::vector<unsigned short>& cGetBiomes() const;

    // Light

    inline glm::ivec4 getLight(unsigned int ind);
    inline unsigned char getLight(unsigned int ind, unsigned char channel);
    inline void setLight(unsigned int ind, unsigned char channel, unsigned char light);
    inline void setBlockLight(unsigned int ind, glm::ivec3 light);

    inline unsigned char getSunlight(unsigned int ind);
    inline void setSunlight(unsigned int ind, unsigned char val);


    // Serialization

    Packet serialize();
    void deserialize(PacketView& packet);

    bool generated = false;
    bool shouldHaveMesh = true;
    bool dirty = true;
    glm::ivec3 pos;
private:
    struct blocklight_bits {
        // 16 bits - 1 short
        unsigned char r: 5;
        unsigned char g: 5;
        unsigned char b: 5, :1;
    };
    struct sunlight_bits {
        // 8 bits for two values - 1 char
        unsigned char a: 4;
        unsigned char b: 4;
    };

    std::vector<unsigned int> blocks {};
    std::vector<unsigned short> biomes {};
    std::array <blocklight_bits, 4096> blocklight {};
    std::array <sunlight_bits, 2048> sunlight {};

    bool empty = true;
    bool partial = false;
    unsigned short nonAirBlocks = 0;

    friend class MapGen;
    void calcNonAirBlocks();
};

inline bool BlockChunk::setBlock(const glm::ivec3& pos, unsigned int blk) {
    if (pos.x > 15 || pos.x < 0 || pos.y > 15 || pos.y < 0 || pos.z > 15 || pos.z < 0) return false;
    return setBlock(Space::Block::index(pos), blk);
}

inline unsigned int BlockChunk::getBlock(unsigned int ind) const {
    if (ind >= 4096) return DefinitionAtlas::INVALID;
    return RIE::read<unsigned int>(ind, blocks, 4096);
}

inline unsigned int BlockChunk::getBlock(const glm::ivec3& pos) const {
    if (pos.x > 15 || pos.x < 0 || pos.y > 15 || pos.y < 0 || pos.z > 15 || pos.z < 0) return DefinitionAtlas::INVALID;
    return getBlock(Space::Block::index(pos));
}

inline bool BlockChunk::setBiome(unsigned int ind, unsigned short bio) {
    RIE::write(ind, bio, biomes, 4096);
}

inline bool BlockChunk::setBiome(const glm::ivec3& pos, unsigned short bio) {
    if (pos.x > 15 || pos.x < 0 || pos.y > 15 || pos.y < 0 || pos.z > 15 || pos.z < 0) return false;
    return setBiome(Space::Block::index(pos), bio);
}

inline unsigned short BlockChunk::getBiome(unsigned int ind) const {
    if (ind >= 4096) return BiomeAtlas::INVALID;
    return RIE::read<unsigned short>(ind, biomes, 4096);
}

inline unsigned short BlockChunk::getBiome(const glm::ivec3& pos) const {
    if (pos.x > 15 || pos.x < 0 || pos.y > 15 || pos.y < 0 || pos.z > 15 || pos.z < 0) return BiomeAtlas::INVALID;
    return getBiome(Space::Block::index(pos));
}

inline glm::ivec4 BlockChunk::getLight(unsigned int ind) {
    return { blocklight[ind].r, blocklight[ind].g, blocklight[ind].b, getSunlight(ind) };
}

inline unsigned char BlockChunk::getLight(unsigned int ind, unsigned char channel) {
    return channel == 0 ? blocklight[ind].r :
           channel == 1 ? blocklight[ind].g :
           channel == 2 ? blocklight[ind].b :
           getSunlight(ind);
}

inline void BlockChunk::setLight(unsigned int ind, unsigned char channel, unsigned char l) {
    channel == 0 ? blocklight[ind].r = l:
    channel == 1 ? blocklight[ind].g = l:
    channel == 2 ? blocklight[ind].b = l:
    (setSunlight(ind, l), 0);
}

inline void BlockChunk::setBlockLight(unsigned int ind, glm::ivec3 l) {
    blocklight[ind].r = l.x;
    blocklight[ind].g = l.y;
    blocklight[ind].b = l.z;
}

inline unsigned char BlockChunk::getSunlight(unsigned int ind) {
    if (ind % 2 == 0) return sunlight[ind / 2].a;
    else return sunlight[ind / 2].b;
}

inline void BlockChunk::setSunlight(unsigned int ind, unsigned char val) {
    if (ind % 2 == 0) sunlight[ind / 2].a = val;
    else sunlight[ind / 2].b = val;
}