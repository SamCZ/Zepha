//
// The base superclass for both dimensions. Handles chunk storage and manipulation.
// Does not include lighting mechanics, which are contained in subclass Dimension.
// Created by aurailus on 2019-11-20.
//

#pragma once

#include <unordered_map>

#include "../util/Vec.h"
#include "../util/CovariantPtr.h"

class World;
class Chunk;
class Region;
class Subgame;
class MapBlock;
class DefinitionAtlas;

class DimensionBase {
public:
    DimensionBase(SubgamePtr game, World& world, const std::string& identifier, unsigned int ind);

    std::string getIdentifier() const;
    unsigned int getInd();

    virtual void update(double delta);

    std::shared_ptr<Region> getRegion(glm::ivec3 regionPosition);
    void removeRegion(glm::ivec3 pos);

    std::shared_ptr<MapBlock> getMapBlock(glm::ivec3 mapBlockPosition);
    virtual void removeMapBlock(glm::ivec3 pos);

    bool mapBlockGenerated(glm::ivec3 mapBlockPosition);

    std::shared_ptr<Chunk> getChunk(glm::ivec3 chunkPosition);
    virtual void setChunk(std::shared_ptr<Chunk> chunk);
    virtual void removeChunk(glm::ivec3 pos);

    unsigned int getBlock(glm::ivec3 pos);
    virtual bool setBlock(glm::ivec3 pos, unsigned int block);

    virtual double getBlockDamage(glm::ivec3 pos) const;
    virtual double setBlockDamage(glm::ivec3 pos, double damage);

    unsigned int getBiome(glm::ivec3 pos);
    virtual bool setBiome(glm::ivec3 pos, unsigned int biome);

    SubgamePtr getGame();
    World& getWorld();

protected:
    virtual void updateBlockDamage(double delta);

    // Combine two chunk partials, or a chunk and a chunk partial.
    // If both are partials `b` takes preference, if one is a fully generated chunk the partial takes preference.
    // TODO: Make this more efficient using proper RIE traversal.
    static std::shared_ptr<Chunk> combinePartials(std::shared_ptr<Chunk> a, std::shared_ptr<Chunk> b);

    SubgamePtr game;
    World& world;

    typedef std::unordered_map<glm::ivec3, std::shared_ptr<Region>, Vec::ivec3> block_region_map;
    block_region_map regions;

    std::string identifier;
    unsigned int ind;

    struct Damage { double curr, max; };
    std::unordered_map<glm::ivec3, Damage, Vec::ivec3> blockDamages;
    
private:
    inline std::shared_ptr<Region> getOrCreateRegion(glm::ivec3 pos);
    inline std::shared_ptr<MapBlock> getOrCreateMapBlock(glm::ivec3 mapBlockPosition);
};
