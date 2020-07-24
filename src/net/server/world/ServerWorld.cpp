//
// Created by aurailus on 05/03/19.
//

#include <iostream>
#include <glm/glm.hpp>
#include <unordered_map>

#include "ServerWorld.h"

#include "ServerGenStream.h"
#include "../../PacketType.h"
#include "../../Serializer.h"
#include "ServerPacketStream.h"
#include "../conn/ClientList.h"
#include "../conn/ServerClient.h"
#include "../../../def/ServerGame.h"
#include "../../../def/item/BlockDef.h"
#include "../../../world/chunk/Chunk.h"
#include "../../../world/chunk/MapBlock.h"
#include "../../../world/fs/FileManipulator.h"
#include "../../../lua/api/class/ServerLuaEntity.h"

ServerWorld::ServerWorld(unsigned int seed, ServerGame& game, ClientList& clients) :
    clientList(clients),
    dimension(game),
    seed(seed),
    game(game) {

    generateOrder.reserve(mapBlockGenRange.x * 2 + 1 * mapBlockGenRange.x * 2 + 1 * mapBlockGenRange.y * 2 + 1);
    std::unordered_set<glm::ivec3, Vec::ivec3> found {};
    std::queue<glm::ivec3> queue {};

    queue.emplace(0, 0, 0);
    found.emplace(0, 0, 0);

    const std::vector<glm::ivec3> dirs {
            glm::ivec3 {1, 0, 0}, glm::ivec3 {-1, 0, 0},
            glm::ivec3 {0, 1, 0}, glm::ivec3 {0, -1, 0},
            glm::ivec3 {0, 0, 1}, glm::ivec3 {0, 0, -1}};

    while (!queue.empty()) {
        glm::ivec3 pos = queue.front();
        queue.pop();

        generateOrder.push_back(pos);

        for (auto dir : dirs) {
            glm::ivec3 offset = pos + dir;
            if (offset.x < -mapBlockGenRange.x || offset.x > mapBlockGenRange.x ||
                offset.y < -mapBlockGenRange.y || offset.y > mapBlockGenRange.y ||
                offset.z < -mapBlockGenRange.x || offset.z > mapBlockGenRange.x ||
                found.count(offset)) continue;
            else {
                found.insert(offset);
                queue.push(offset);
            }
        }
    }
}

void ServerWorld::init(const std::string& worldDir) {
    genStream = std::make_unique<ServerGenStream>(seed, game);
    packetStream = std::make_unique<ServerPacketStream>(dimension);
    fileManip = std::make_shared<FileManipulator>("worlds/" + worldDir + "/");

    generateMapBlock({0, 0, 0});
}

void ServerWorld::update(double delta) {
    dimension.update(clientList.clients, mapBlockGenRange);

    std::unordered_set<glm::ivec3, Vec::ivec3> updatedChunks {};
    std::unordered_set<glm::ivec3, Vec::ivec3> generatedMapBlocks {};

    auto finishedGen = genStream->update();
    for (auto& data : *finishedGen) {

        for (const auto& chunk : data.chunks) {
            generatedMapBlocks.insert(data.pos);
            updatedChunks.insert(chunk->pos);
            dimension.setChunk(chunk);
        }

//        auto resend = dimension.calculateEdgeLight(mb.pos);
//        changed.insert(resend.begin(), resend.end());

        dimension.getMapBlock(data.pos)->generated = true;
        packetStream->queue(data.pos);
    }

    auto finishedPackets = packetStream->update();
    for (auto& data : *finishedPackets) {
        for (auto& client : clientList.clients) {
            if (!client->hasPlayer) continue;
            data->packet->sendTo(client->peer, PacketChannel::WORLD);
        }
    }

    this->generatedMapBlocks = generatedMapBlocks.size();

//    for (auto& chunkPos : updatedChunks) {
//        glm::ivec3 mapBlockPos = Space::MapBlock::world::fromChunk(chunkPos);
//        auto chunk = dimension.getChunk(chunkPos);
//
//        assert(chunk != nullptr);
//
//        bool sentAlready = false;
//        for (auto& mapBlock : generatedMapBlocks) if (mapBlock == mapBlockPos) { sentAlready = true; break; }
//        if (sentAlready) continue;
//
//        Packet p(PacketType::CHUNK);
//        auto l = chunk->aquireLock();
//        p.data = chunk->serialize();
//        l.release();
//
//        for (auto& client : clientList.clients) {
//            if (!client->hasPlayer) continue;
//
//            auto myChunk = Space::Chunk::world::fromBlock(client->getPos());
//
//            std::pair<glm::ivec3, glm::ivec3> bounds = {
//                {myChunk.x - activeChunkRange.x, myChunk.y - activeChunkRange.y, myChunk.z - activeChunkRange.x},
//                {myChunk.x + activeChunkRange.x, myChunk.y + activeChunkRange.y, myChunk.z + activeChunkRange.x}};
//
//            if (isInBounds(chunkPos, bounds)) p.sendTo(client->peer, PacketChannel::WORLD);
//        }
//    }

    // Send the # of generated chunks to the client (debug),
    // and trigger new chunks to be generated if a player has changed MapBlocks.
    Packet r = Serializer().append(this->generatedMapBlocks).packet(PacketType::SERVER_INFO);

    for (auto& client : clientList.clients) {
        if (client->hasPlayer) {
            r.sendTo(client->peer, PacketChannel::SERVER);
            if (client->changedMapBlocks) changedMapBlocks(*client);
        }
    }

    // Send all dirty entities to all clients
    // TODO: Only send to *nearby clients*.
    for (auto& entity : dimension.getLuaEntities()) {
        if (entity->entity->checkAndResetDirty()) {
            Packet p = entity->entity->createPacket();
            for (auto& client : clientList.clients) {
                if (client->hasPlayer) p.sendTo(client->peer, PacketChannel::ENTITY);
            }
        }
    }

    for (unsigned int entity : dimension.getRemovedEntities()) {
        Packet p = Serializer()
        .append<unsigned int>(entity)
        .packet(PacketType::ENTITY_REMOVED);

        for (auto& client : clientList.clients) {
            if (client->hasPlayer) p.sendTo(client->peer, PacketChannel::ENTITY);
        }
    }

    dimension.clearRemovedEntities();
}

void ServerWorld::changedMapBlocks(ServerClient& client) {
    generateMapBlocks(client);
    sendChunksToPlayer(client);
    client.changedMapBlocks = false;
}

void ServerWorld::generateMapBlocks(ServerClient& client) {
    unsigned int generating = 0;
    glm::ivec3 playerMapBlock = Space::MapBlock::world::fromBlock(client.getPos());

    for (const auto &c : generateOrder) {
        glm::ivec3 mapBlockPos = playerMapBlock + c;
        auto existing = dimension.getMapBlock(mapBlockPos);
        if (existing && existing->generated) continue;
        else generating += generateMapBlock(mapBlockPos);
    }

    std::cout << "Player moved, generating " << generating << " MapBlocks." << std::endl;
}

bool ServerWorld::generateMapBlock(glm::ivec3 pos) {
    if(!dimension.getMapBlock(pos) || !dimension.getMapBlock(pos)->generated) return genStream->queue(pos);
    return false;
}

void ServerWorld::sendChunksToPlayer(ServerClient& client) {
    glm::ivec3 playerPos = Space::MapBlock::world::fromBlock(client.getPos());
    std::pair<glm::ivec3, glm::ivec3> bounds = {
        {playerPos.x - sendRange.x, playerPos.y - sendRange.y, playerPos.z - sendRange.x},
        {playerPos.x + sendRange.x, playerPos.y + sendRange.y, playerPos.z + sendRange.x}};

    glm::ivec3 lastPlayerPos = Space::MapBlock::world::fromBlock(client.lastPos);
    std::pair<glm::ivec3, glm::ivec3> oldBounds = {
        {lastPlayerPos.x - sendRange.x, lastPlayerPos.y - sendRange.y, lastPlayerPos.z - sendRange.x},
        {lastPlayerPos.x + sendRange.x, lastPlayerPos.y + sendRange.y, lastPlayerPos.z + sendRange.x}};

    for (int i = bounds.first.x; i < bounds.second.x; i++) {
        for (int j = bounds.first.y; j < bounds.second.y; j++) {
            for (int k = bounds.first.z; k < bounds.second.z; k++) {
                glm::ivec3 pos {i, j, k};
                if (isInBounds(pos, oldBounds)) continue;
                packetStream->queue(pos);
            }
        }
    }
}

unsigned int ServerWorld::getBlock(glm::ivec3 pos) {
    return dimension.getBlock(pos);
}

void ServerWorld::setBlock(glm::ivec3 pos, unsigned int block) {
    auto oldBlock = getBlock(pos);

    if (block == DefinitionAtlas::AIR) {
        auto def = game.defs.blockFromId(oldBlock);
        if (def.callbacks.count(Callback::DESTRUCT)) def.callbacks[Callback::DESTRUCT](pos);
    }
    else {
        auto def = game.defs.blockFromId(block);
        if (def.callbacks.count(Callback::CONSTRUCT)) def.callbacks[Callback::CONSTRUCT](pos);
    }

    dimension.setBlock(pos, block);

    Packet b = Serializer().append(pos).append(block).packet(PacketType::BLOCK_SET);
    auto chunkPos = Space::Chunk::world::fromBlock(pos);

    for (auto &client : clientList.clients) {
        if (client->hasPlayer) {
            glm::ivec3 mapBlock = Space::MapBlock::world::fromBlock(client->getPos());
            std::pair<glm::ivec3, glm::ivec3> bounds = {
                {mapBlock.x - mapBlockGenRange.x, mapBlock.y - mapBlockGenRange.y, mapBlock.z - mapBlockGenRange.x},
                {mapBlock.x + mapBlockGenRange.x, mapBlock.y + mapBlockGenRange.y, mapBlock.z + mapBlockGenRange.x}};

            if (isInBounds(Space::MapBlock::world::fromChunk(chunkPos), bounds))
                b.sendTo(client->peer, PacketChannel::BLOCK);
        }
    }

    if (block == DefinitionAtlas::AIR) {
        auto def = game.defs.blockFromId(oldBlock);
        if (def.callbacks.count(Callback::AFTER_DESTRUCT)) def.callbacks[Callback::AFTER_DESTRUCT](pos);
    }
    else {
        auto def = game.defs.blockFromId(block);
        if (def.callbacks.count(Callback::AFTER_CONSTRUCT)) def.callbacks[Callback::AFTER_CONSTRUCT](pos);
    }
}

bool ServerWorld::isInBounds(glm::ivec3 cPos, std::pair<glm::ivec3, glm::ivec3> &bounds) {
    return (cPos.x >= bounds.first.x && cPos.x <= bounds.second.x
         && cPos.y >= bounds.first.y && cPos.y <= bounds.second.y
         && cPos.z >= bounds.first.z && cPos.z <= bounds.second.z);
}