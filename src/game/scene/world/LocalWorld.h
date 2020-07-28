//
// Created by aurailus on 14/12/18.
//

#pragma once

#include "World.h"

#include "../../../world/LocalDimension.h"

class Target;
class Player;
class Renderer;
class ItemStack;
class LocalSubgame;
class ParticleEntity;
class BlockCrackEntity;
class ClientNetworkInterpreter;
class WorldInterpolationStream;

class LocalWorld : public World {
public:
    LocalWorld(LocalSubgame& defs, ClientNetworkInterpreter* net);

    void init(Player* player);
    void update(double delta) override;

    void createDimension(std::string identifier);

    void loadWorldPacket(std::unique_ptr<PacketView> p);
    void commitChunk(std::shared_ptr<Chunk> chunk);

    unsigned int getBlock(glm::ivec3 pos) override;
    void setBlock(glm::ivec3 pos, unsigned int block) override;

    double setBlockDamage(glm::ivec3 pos, double damage) override;

    void blockPlace(Target& target);
    void blockPlaceOrInteract(Target& target);
    void blockInteract(Target& target);
    double blockHit(Target& target);

    unsigned short getBiome(glm::vec3 pos);
    std::shared_ptr<Chunk> getChunk(glm::ivec3 pos);

    int getMeshChunkCount();
    int renderChunks(Renderer &render);
    void renderEntities(Renderer &renderer);

    LocalSubgame& game;
    LocalDimension dimension;

    int mapBlocksInterpolated = 0;
    int lastMeshUpdates = 0;
private:
    void finishChunks();
    void updateBlockDamages(double delta);

    std::unordered_map<glm::ivec3, BlockCrackEntity*, Vec::ivec3> crackEntities;
    std::vector<ParticleEntity*> particles;

    Player* player = nullptr;
    ClientNetworkInterpreter* net = nullptr;
    WorldInterpolationStream* worldGenStream = nullptr;
};
