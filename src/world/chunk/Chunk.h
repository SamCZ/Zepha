//
// The Chunk data class that contains the block, biome, and light data.
// Created by aurailus on 14/12/18.
//

#pragma once

#include <mutex>
#include <array>
#include <vector>
#include <glm/vec3.hpp>

#include "../../util/RIE.h"
#include "../../util/Space.h"

class Deserializer;

class Chunk {
public:
    friend class MapGen;

    struct BlockLight {
        // 16 bits - 1 short
        unsigned char r: 5;
        unsigned char g: 5;
        unsigned char b: 5, :1;
    };
    typedef union { short sh; BlockLight b; } blocklight_union;

    struct SunLight {
        // 8 bits for two values - 1 char
        unsigned char a: 4;
        unsigned char b: 4;
    };
    typedef union { char ch; SunLight s; } sunlight_union;

    Chunk() = default;
    Chunk(const Chunk& o);
    Chunk(glm::ivec3 pos, bool partial = false);
    Chunk(glm::ivec3 pos, const std::vector<unsigned int>& blocks, const std::vector<unsigned short>& biomes);

    inline std::unique_lock<std::mutex> aquireLock();

    inline unsigned int getBlock(unsigned int ind) const;
    bool setBlock(unsigned int ind, unsigned int blk);

    inline unsigned int getBlock(const glm::ivec3& pos) const;
    inline bool setBlock(const glm::ivec3& pos, unsigned int blk);

    inline unsigned short getBiome(unsigned int ind) const;
    inline bool setBiome(unsigned int ind, unsigned short bio);

    inline unsigned short getBiome(const glm::ivec3& pos) const;
    inline bool setBiome(const glm::ivec3& pos, unsigned short bio);

    const std::vector<unsigned int>& cGetBlocks() const;
    const std::vector<unsigned short>& cGetBiomes() const;

    inline glm::ivec4 getLight(unsigned int ind);
    inline void setLight(unsigned int ind, glm::ivec4 light);

    inline unsigned char getLight(unsigned int ind, unsigned char channel);
    inline void setLight(unsigned int ind, unsigned char channel, unsigned char light);

    std::string serialize();
    void deserialize(Deserializer& d);

    void countRenderableBlocks();

    bool partial = false;
    bool generated = false;

    bool dirty = true;
    bool shouldRender = true;

    glm::ivec3 pos {};
private:
    std::mutex m;

    std::vector<unsigned int> blocks {0, 0};
    std::vector<unsigned short> biomes {0, 0};

    std::array<SunLight, 2048> sunLight {};
    std::array<BlockLight, 4096> blockLight {};

    unsigned short renderableBlocks = 0;

    inline unsigned char getSunlight(unsigned int ind);
    inline void setSunlight(unsigned int ind, unsigned char val);
};

std::unique_lock<std::mutex> Chunk::aquireLock() {
    std::unique_lock<std::mutex> lock(m, std::defer_lock);
    return std::move(lock);
}

inline unsigned int Chunk::getBlock(const glm::ivec3& pos) const {
    if (pos.x > 15 || pos.x < 0 || pos.y > 15 || pos.y < 0 || pos.z > 15 || pos.z < 0) return 0; // Invalid
    return getBlock(Space::Block::index(pos));
}

inline bool Chunk::setBlock(const glm::ivec3& pos, unsigned int blk) {
    if (pos.x > 15 || pos.x < 0 || pos.y > 15 || pos.y < 0 || pos.z > 15 || pos.z < 0) return false;
    return setBlock(Space::Block::index(pos), blk);
}

inline unsigned int Chunk::getBlock(unsigned int ind) const {
    if (ind >= 4096) return 0; // Invalid
    return RIE::read<unsigned int>(ind, blocks, 4096);
}

inline unsigned short Chunk::getBiome(unsigned int ind) const {
    if (ind >= 4096) return 0; // Invalid
    return RIE::read<unsigned short>(ind, biomes, 4096);
}

inline bool Chunk::setBiome(unsigned int ind, unsigned short bio) {
    return RIE::write(ind, bio, biomes, 4096);
}

inline unsigned short Chunk::getBiome(const glm::ivec3& pos) const {
    if (pos.x > 15 || pos.x < 0 || pos.y > 15 || pos.y < 0 || pos.z > 15 || pos.z < 0) return 0; // Invalid
    return getBiome(Space::Block::index(pos));
}

inline bool Chunk::setBiome(const glm::ivec3& pos, unsigned short bio) {
    if (pos.x > 15 || pos.x < 0 || pos.y > 15 || pos.y < 0 || pos.z > 15 || pos.z < 0) return false;
    return setBiome(Space::Block::index(pos), bio);
}

inline glm::ivec4 Chunk::getLight(unsigned int ind) {
    return { blockLight[ind].r, blockLight[ind].g, blockLight[ind].b, getSunlight(ind) };
}

inline void Chunk::setLight(unsigned int ind, glm::ivec4 l) {
    blockLight[ind].r = l.x;
    blockLight[ind].g = l.y;
    blockLight[ind].b = l.z;
    setSunlight(ind, l.w);
}

inline unsigned char Chunk::getLight(unsigned int ind, unsigned char channel) {
    return channel == 0 ? blockLight[ind].r :
           channel == 1 ? blockLight[ind].g :
           channel == 2 ? blockLight[ind].b :
           getSunlight(ind);
}

inline void Chunk::setLight(unsigned int ind, unsigned char channel, unsigned char l) {
    channel == 0 ? blockLight[ind].r = l :
    channel == 1 ? blockLight[ind].g = l :
    channel == 2 ? blockLight[ind].b = l :
    (setSunlight(ind,l), 0);
}

inline unsigned char Chunk::getSunlight(unsigned int ind) {
    if (ind % 2 == 0) return sunLight[ind / 2].a;
    else return sunLight[ind / 2].b;
}

inline void Chunk::setSunlight(unsigned int ind, unsigned char val) {
    if (ind % 2 == 0) sunLight[ind / 2].a = val;
    else sunLight[ind / 2].b = val;
}