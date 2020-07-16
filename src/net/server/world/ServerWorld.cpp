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

    generateOrder.reserve(MB_GEN_H * 2 + 1 * MB_GEN_H * 2 + 1 * MB_GEN_V * 2 + 1);
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
            if (offset.x < -MB_GEN_H || offset.x > MB_GEN_H ||
                offset.y < -MB_GEN_V || offset.y > MB_GEN_V ||
                offset.z < -MB_GEN_H || offset.z > MB_GEN_H ||
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
    fileManip = std::make_shared<FileManipulator>("worlds/" + worldDir + "/");

    generateMapBlock({0, 0, 0});
}

void ServerWorld::update(double delta) {
    dimension.update(clientList.clients);

    auto finished = genStream->update();
    generatedMapBlocks = static_cast<int>(finished->size());

    std::unordered_set<glm::ivec3, Vec::ivec3> changed {};

    for (auto& data : *finished) {
        for (const auto& chunk : data.chunks) {
            changed.insert(chunk->pos);
            dimension.setChunk(chunk);
//            fileManip->commitChunk(*chunk);
        }

//        auto resend = dimension.calculateEdgeLight(mb.pos);
//        changed.insert(resend.begin(), resend.end());

        auto mb = dimension.getMapBlock(data.pos)->generated = true;
    }

    for (auto& chunk : changed) {
        for (auto& client : clientList.clients) {
            if (!client->hasPlayer) continue;

            auto myChunk = Space::Chunk::world::fromBlock(client->getPos());

            std::pair<glm::ivec3, glm::ivec3> bounds = {
                {myChunk.x - CHUNK_SEND_H, myChunk.y - CHUNK_SEND_V, myChunk.z - CHUNK_SEND_H},
                {myChunk.x + CHUNK_SEND_H, myChunk.y + CHUNK_SEND_V, myChunk.z + CHUNK_SEND_H}};

            if (isInBounds(chunk, bounds)) sendChunk(dimension.getChunk(chunk), *client);
        }
    }

    // Send the # of generated chunks to the client (debug),
    // and trigger new chunks to be generated if a player has changed MapBlocks.
    Packet r(PacketType::SERVER_INFO);
    r.data = Serializer().append(generatedMapBlocks).data;

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
    glm::ivec3 playerChunk = Space::Chunk::world::fromBlock(client.getPos());
    std::pair<glm::ivec3, glm::ivec3> bounds = {
        {playerChunk.x - CHUNK_SEND_H, playerChunk.y - CHUNK_SEND_V, playerChunk.z - CHUNK_SEND_H},
        {playerChunk.x + CHUNK_SEND_H, playerChunk.y + CHUNK_SEND_V, playerChunk.z + CHUNK_SEND_H}};

    glm::ivec3 lastPlayerChunk = Space::Chunk::world::fromBlock(client.lastPos);
    std::pair<glm::ivec3, glm::ivec3> oldBounds = {
        {lastPlayerChunk.x - CHUNK_SEND_H, lastPlayerChunk.y - CHUNK_SEND_V, lastPlayerChunk.z - CHUNK_SEND_H},
        {lastPlayerChunk.x + CHUNK_SEND_H, lastPlayerChunk.y + CHUNK_SEND_V, lastPlayerChunk.z + CHUNK_SEND_H}};

    for (int i = bounds.first.x; i < bounds.second.x; i++) {
        for (int j = bounds.first.y; j < bounds.second.y; j++) {
            for (int k = bounds.first.z; k < bounds.second.z; k++) {
                glm::ivec3 chunkPos {i, j, k};
                if (isInBounds(chunkPos, oldBounds)) continue;
                auto chunk = dimension.getChunk(chunkPos);
                if (chunk) sendChunk(chunk, client);
            }
        }
    }
}

void ServerWorld::sendChunk(const std::shared_ptr<Chunk>& chunk, ServerClient &peer) {
    if (chunk == nullptr || !chunk->generated) return;

    Packet r(PacketType::CHUNK);

    auto l = chunk->aquireLock();
    r.data = chunk->serialize();
    l.release();

    r.sendTo(peer.peer, PacketChannel::CHUNK);
}

unsigned int ServerWorld::getBlock(glm::ivec3 pos) {
    return dimension.getBlock(pos);
}

void ServerWorld::setBlock(glm::ivec3 pos, unsigned int block) {
    auto oldBlock = getBlock(pos);

    if (block == DefinitionAtlas::AIR) {
        auto def = game.defs.blockFromId(oldBlock);
        if (def.callbacks.count(Callback::DESTRUCT)) {
            def.callbacks[Callback::DESTRUCT](game.parser.luaVec(pos));
        }
    }
    else {
        auto def = game.defs.blockFromId(block);
        if (def.callbacks.count(Callback::CONSTRUCT)) {
            def.callbacks[Callback::CONSTRUCT](game.parser.luaVec(pos));
        }
    }

    dimension.setBlock(pos, block);

    Packet b = Serializer().append(pos).append(block).packet(PacketType::BLOCK_SET);
    auto chunkPos = Space::Chunk::world::fromBlock(pos);

    for (auto &client : clientList.clients) {
        if (client->hasPlayer) {
            glm::ivec3 mapBlock = Space::MapBlock::world::fromBlock(client->getPos());
            std::pair<glm::ivec3, glm::ivec3> bounds = {
                {mapBlock.x - MB_GEN_H, mapBlock.y - MB_GEN_V, mapBlock.z - MB_GEN_H},
                {mapBlock.x + MB_GEN_H, mapBlock.y + MB_GEN_V, mapBlock.z + MB_GEN_H}};

            if (isInBounds(Space::MapBlock::world::fromChunk(chunkPos), bounds))
                b.sendTo(client->peer, PacketChannel::BLOCK);
        }
    }

    if (block == DefinitionAtlas::AIR) {
        auto def = game.defs.blockFromId(oldBlock);
        if (def.callbacks.count(Callback::AFTER_DESTRUCT))
            def.callbacks[Callback::AFTER_DESTRUCT](game.parser.luaVec(pos));
    }
    else {
        auto def = game.defs.blockFromId(block);
        if (def.callbacks.count(Callback::AFTER_CONSTRUCT))
            def.callbacks[Callback::AFTER_CONSTRUCT](game.parser.luaVec(pos));
    }
}

bool ServerWorld::isInBounds(glm::ivec3 cPos, std::pair<glm::ivec3, glm::ivec3> &bounds) {
    return (cPos.x >= bounds.first.x && cPos.x <= bounds.second.x
         && cPos.y >= bounds.first.y && cPos.y <= bounds.second.y
         && cPos.z >= bounds.first.z && cPos.z <= bounds.second.z);
}