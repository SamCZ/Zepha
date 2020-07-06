//
// World subclass for the server.
// Handles blocks, entities, and clients.
// Created by aurailus on 05/03/19.
//

#pragma once

#include "../../game/scene/world/World.h"

#include "../../world/ServerDimension.h"

class ServerGame;
class ClientList;
class ServerClient;
class FileManipulator;
class ServerGenStream;

class ServerWorld : public World {
public:
    const static int MB_GEN_H = 4, MB_GEN_V = 4;
    const static int CHUNK_SEND_H = 10, CHUNK_SEND_V = 10;

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

    static void sendChunk(const std::shared_ptr<Chunk>& chunk, ServerClient& client);

    std::shared_ptr<ServerGenStream> genStream = nullptr;

    unsigned int seed;
    ServerGame& game;
    ClientList& clientList;
    
    std::string worldDir;
    std::shared_ptr<FileManipulator> fileManip;

    unsigned int generatedMapBlocks = 0;
    std::vector<glm::ivec3> generateOrder;
};

