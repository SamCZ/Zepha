//
// Created by aurailus on 14/12/18.
//

#pragma once

#include "World.h"

#include "../../../world/LocalDimension.h"

class ClientNetworkInterpreter;
class WorldInterpolationStream;
class BlockCrackEntity;
class ParticleEntity;
class PointedThing;
class LocalSubgame;
class Renderer;
class Player;

class LocalWorld : public World {
public:
    LocalWorld(LocalSubgame& defs, ClientNetworkInterpreter* net);

    void init(Player* player);
    void update(double delta) override;

    void loadWorldPacket(std::unique_ptr<PacketView> p);
    void commitChunk(std::shared_ptr<Chunk> chunk);

    unsigned int getBlock(glm::ivec3 pos) override;
    void setBlock(glm::ivec3 pos, unsigned int block) override;

    void blockPlace(glm::vec3 pos, unsigned int block);
    void blockBreak(glm::vec3 pos);

    void blockInteract(PointedThing& thing);
    double blockHit(PointedThing& thing);

    unsigned short getBiome(glm::vec3 pos);
    std::shared_ptr<Chunk> getChunk(glm::ivec3 pos);

    int getMeshChunkCount();
    int renderChunks(Renderer &render);
    void renderEntities(Renderer &renderer);

    LocalSubgame& defs;
    LocalDimension dimension;

    int mapBlocksInterpolated = 0;
    int lastMeshUpdates = 0;
private:
    void finishChunks();
    void updateBlockDamages(double delta);

    std::vector<BlockCrackEntity*> crackedBlocks;
    std::vector<ParticleEntity*> particles;

    Player* player = nullptr;
    ClientNetworkInterpreter* net = nullptr;
    WorldInterpolationStream* worldGenStream = nullptr;
};
