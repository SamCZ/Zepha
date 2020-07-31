//
// World subclass for the server.
// Handles blocks, entities, and clients.
// Created by aurailus on 05/03/19.
//

#pragma once

#include "../../../game/scene/world/World.h"

#include "../../../world/ServerDimension.h"

class Target;
class ItemStack;
class ServerClients;
class ServerPlayer;
class ServerInventoryRefs;
class ServerSubgame;
class ServerGenStream;
class ServerPacketStream;

class ServerWorld : public World {
public:
    explicit ServerWorld(unsigned int seed, ServerSubgame& game, ServerClients& clients);

    void init(const std::string& worldDir);
    void update(double delta) override;

    virtual ServerDimension& createDimension(const std::string& identifier) override;

    virtual ServerDimension& getDimension(unsigned int index) override;
    virtual ServerDimension& getDimension(const std::string& identifier) override;

    std::shared_ptr<ServerDimension> getDefaultDimensionPtr();
    std::shared_ptr<ServerDimension> getDimensionPtr(const std::string& identifier);

    std::shared_ptr<ServerInventoryRefs> getRefs();
private:
    void changedMapBlocks(ServerPlayer& player);
    static bool isInBounds(glm::ivec3 pos, std::pair<glm::ivec3, glm::ivec3>& bounds);

    bool generateMapBlock(unsigned int dim, glm::ivec3 pos);
    void generateMapBlocks(ServerPlayer& player);
    void sendChunksToPlayer(ServerPlayer& client);

    std::shared_ptr<ServerGenStream> genStream = nullptr;
    std::shared_ptr<ServerPacketStream> packetStream = nullptr;

    unsigned int seed;
    ServerClients& clients;
    std::shared_ptr<ServerInventoryRefs> refs;

//    std::string worldDir;
//    std::shared_ptr<FileManipulator> fileManip;

    unsigned int generatedMapBlocks = 0;
    std::vector<glm::ivec3> generateOrder;

    const glm::ivec2 mapBlockGenRange = {4, 4};
    const glm::ivec2 sendRange        = {4, 4};
    const glm::ivec2 activeChunkRange = {16, 16};
};

