//
// The Chunk data class that contains the block, biome, and light data.
// Created by aurailus on 14/12/18.
//

#pragma once

#include <mutex>
#include <array>
#include <vector>
#include <glm/vec3.hpp>

#include "util/Lockable.h"

#include "util/RIE.h"
#include "util/Space.h"

class Deserializer;

class Chunk : public Lockable {
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

    inline glm::ivec3 getPos() const;
    inline void setPos(glm::ivec3 pos);

    inline bool isDirty() const;
    inline void setDirty(bool dirty);

    inline bool chunkShouldRender() const;
    inline void setShouldRender(bool shouldRender);

    inline bool isPartial() const;
    inline void setPartial(bool partial);

    inline bool isGenerated() const;
    inline void setGenerated(bool generated);

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
private:
    glm::ivec3 pos {};

    bool partial = false;
    bool generated = false;

    bool dirty = true;
    bool shouldRender = true;

    std::vector<unsigned int> blocks {0, 0};
    std::vector<unsigned short> biomes {0, 0};

    std::array<SunLight, 2048> sunLight {};
    std::array<BlockLight, 4096> blockLight {};

    unsigned short renderableBlocks = 0;

    inline unsigned char getSunlight(unsigned int ind);
    inline void setSunlight(unsigned int ind, unsigned char val);
};

#include "Chunk.inl"