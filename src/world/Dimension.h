//
// DimensionBase subclass that handles block light and sunlight.
// This class was split from DimensionBase to help differentiate
// lighting functions from basic world manipulation.
// Created by aurailus on 2020-05-26.
//

#pragma once

#include <queue>
#include <unordered_set>

#include "DimensionBase.h"

class Target;
class Player;

class Dimension : public DimensionBase {
public:
    typedef std::unordered_set<glm::ivec3, Vec::ivec3> relitChunks;

    Dimension(const Dimension& o) = delete;
    Dimension(SubgamePtr game, World& world, const std::string& identifier, unsigned int ind) :
        DimensionBase(game, world, identifier, ind) {}

    // Override setBlock to update lighting.
    bool setBlock(glm::ivec3 pos, unsigned int block) override;

    virtual void blockPlace(const Target &target, PlayerPtr player) = 0;
    virtual void blockPlaceOrInteract(const Target &target, PlayerPtr player) = 0;
    virtual void blockInteract(const Target &target, PlayerPtr player) = 0;
    virtual double blockHit(const Target &target, PlayerPtr player) = 0;

    unsigned int nextEntityInd();

    // Calculate light propogation around MapBlock edges,
    // Called after a new mapblock is inserted into the dimension.
    relitChunks calculateEdgeLight(glm::ivec3 mbPos);
protected:

    // Lighting propagation.

    virtual relitChunks propogateAddNodes();
    virtual relitChunks propogateRemoveNodes();
private:

    // Other Lighting methods.

    static inline bool containsWorldPos(Chunk* chunk, glm::ivec3 pos);
    inline glm::ivec4 getLight(glm::ivec3 worldPos, Chunk* chunk = nullptr);

    void calculateHorizontalEdge(std::shared_ptr<Chunk> a, std::shared_ptr<Chunk> b);
    void calculateVerticalEdge(std::shared_ptr<Chunk> above, std::shared_ptr<Chunk> below);

    inline void addBlockLight(glm::ivec3 pos, glm::ivec3 light);
    inline void removeBlockLight(glm::ivec3 pos);

    inline void reflowLight(glm::ivec3 pos);
    inline void removeSunlight(glm::ivec3 pos);
    inline void setAndReflowSunlight(glm::ivec3 pos, unsigned char level);

    struct LightAddNode {
        LightAddNode(unsigned short index, Chunk* chunk) : index(index), chunk(chunk) {};
        unsigned short index; Chunk* chunk;
    };
    struct LightRemoveNode {
        LightRemoveNode(unsigned short index, unsigned short value, Chunk* chunk) : index(index), value(value), chunk(chunk) {};
        unsigned short index, value; Chunk* chunk;
    };

    static constexpr unsigned char SUNLIGHT_CHANNEL = 3;
    std::array<std::queue<LightAddNode>, 4> lightAddQueue;
    std::array<std::queue<LightRemoveNode>, 4> lightRemoveQueue;

    unsigned int entityInd = 0;
};
