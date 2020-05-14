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

    bool setBlock(unsigned int ind, unsigned int blk);
    inline bool setBlock(const glm::ivec3& pos, unsigned int blk);

    inline unsigned int getBlock(unsigned int ind) const;
    inline unsigned int getBlock(const glm::ivec3& pos) const;

    inline bool setBiome(unsigned int ind, unsigned short bio);
    inline bool setBiome(const glm::ivec3& pos, unsigned short bio);

    inline unsigned short getBiome(unsigned int ind) const;
    inline unsigned short getBiome(const glm::ivec3& pos) const;

    inline void setSunlight(unsigned int ind, unsigned char val);
    inline void setBlockLight(unsigned int ind, glm::ivec3 light);
    inline void setBlockLight(unsigned int ind, unsigned char channel, unsigned char light);

    inline unsigned char getSunlight(unsigned int ind);
    inline glm::ivec3 getBlockLight(unsigned int ind);
    inline unsigned char getBlockLight(unsigned int ind, unsigned char channel);

    inline glm::vec4 getLightVec(unsigned int ind);

    const std::vector<unsigned int>& cGetBlocks() const;
    const std::vector<unsigned short>& cGetBiomes() const;

    Packet serialize();
    void deserialize(PacketView& packet);

    bool shouldHaveMesh = true;
    bool dirty = true;

    bool generated = false;

    glm::ivec3 pos;
private:
    struct light_bits {
        // 16 bits - 1 short
        unsigned char r: 5;
        unsigned char g: 5;
        unsigned char b: 5, :1;
    };

    std::vector<unsigned int> blocks {};
    std::vector<unsigned short> biomes {};
    std::array <light_bits, 4096> light {};

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

inline void BlockChunk::setSunlight(unsigned int ind, unsigned char val) {
}

inline void BlockChunk::setBlockLight(unsigned int ind, glm::ivec3 l) {
    light[ind].r = l.x;
    light[ind].g = l.y;
    light[ind].b = l.z;
}

inline void BlockChunk::setBlockLight(unsigned int ind, unsigned char channel, unsigned char l){
    channel == 0 ? light[ind].r = l:
    channel == 1 ? light[ind].g = l:
                   light[ind].b = l;
}

inline unsigned char BlockChunk::getSunlight(unsigned int ind) {
    return 15;
}

inline glm::ivec3 BlockChunk::getBlockLight(unsigned int ind) {
    return { light[ind].r, light[ind].g, light[ind].b };
}

inline unsigned char BlockChunk::getBlockLight(unsigned int ind, unsigned char channel) {
    return channel == 0 ? light[ind].r:
           channel == 1 ? light[ind].g:
                          light[ind].b;
}

inline glm::vec4 BlockChunk::getLightVec(unsigned int ind) {
    return glm::vec4 { light[ind].r, light[ind].g, light[ind].b, getSunlight(ind) };
}