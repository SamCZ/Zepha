//
// World subclass for the server.
// Handles blocks, entities, and clients.
// Created by aurailus on 05/03/19.
//

#pragma once

#include "../../../game/scene/world/World.h"

#include "../../../world/ServerDimension.h"

class ServerGame;
class ClientList;
class ServerClient;
class FileManipulator;
class ServerGenStream;
class ServerPacketStream;

class ServerWorld : public World {
public:
    explicit ServerWorld(unsigned int seed, ServerGame& game, ClientList& clients);

    void init(const std::string& worldDir);
    void update(double delta) override;

    unsigned int getBlock(glm::ivec3 pos) override;
    void setBlock(glm::ivec3 pos, unsigned int block) override;

    ServerDimension dimension;
private:
    void changedMapBlocks(ServerClient& client);
    static bool isInBounds(glm::ivec3 pos, std::pair<glm::ivec3, glm::ivec3>& bounds);

    void generateMapBlocks(ServerClient& client);
    bool generateMapBlock(glm::ivec3 pos);
    void sendChunksToPlayer(ServerClient& client);

    std::shared_ptr<ServerGenStream> genStream = nullptr;
    std::shared_ptr<ServerPacketStream> packetStream = nullptr;

    unsigned int seed;
    ServerGame& game;
    ClientList& clientList;
    
    std::string worldDir;
    std::shared_ptr<FileManipulator> fileManip;

    unsigned int generatedMapBlocks = 0;
    std::vector<glm::ivec3> generateOrder;

    const glm::ivec2 mapBlockGenRange = {6, 4};
    const glm::ivec2 sendRange        = {6, 4};
    const glm::ivec2 activeChunkRange = {24, 24};
};

