//
// Created by aurailus on 05/03/19.
//

#include <algorithm>
#include <glm/glm.hpp>
#include <unordered_map>

#include "WorldGenStream.h"
#include "../conn/ServerClient.h"
#include "../../world/chunk/BlockChunk.h"
#include "../../util/Vec.h"

#include "ServerWorld.h"

ServerWorld::ServerWorld(unsigned int seed, ServerGame& defs, ClientList& clients) :
    clientList(clients),
    seed(seed),
    defs(defs) {

    //Pregenerate chunk generation order
    generateOrder.reserve(MB_GEN_H * 2 * MB_GEN_H * 2 * MB_GEN_V * 2);

    for (int i = 0; i <= MB_GEN_H; i++) {
        for (int j = 0; j <= i; j++) {
            for (int k = -MB_GEN_V; k <= MB_GEN_V; k++) {
                for (int l = -1; l <= 1; l += 2) {
                    for (int m = -1; m <= 1; m += 2) {
                        for (int n = 0; n <= 1; n++) {
                            generateOrder.emplace_back((n ? l*i : m*j), k, (n ? m*j : l*i));
                        }
                    }
                }
            }
        }
    }
}

void ServerWorld::init() {
    genStream = std::make_unique<WorldGenStream>(seed, defs);
}

void ServerWorld::update(double delta) {
    dimension.update(clientList.clients);

    while (!generateQueueList.empty()) {
        auto it = generateQueueList.begin();
        glm::vec3 pos = *it;

        if (genStream->tryToQueue(pos)) {
            generateQueueList.erase(it);
            generateQueueMap.erase(pos);
        }
        else break;
    }

    auto finished = genStream->update();
    generatedChunks = static_cast<int>(finished->size());

    for (const auto& chunk : *finished) {
        dimension.setChunk(chunk);

        glm::ivec3 mapBlockPos = Space::MapBlock::world::fromChunk(chunk->pos);
        if (chunk->generated) dimension.getMapBlock(mapBlockPos)->generated = true;
        unsigned long long mapBlockIntegrity = dimension.getMapBlockIntegrity(mapBlockPos);

        for (auto& client : clientList.clients) {
            if (client->hasPlayer) {
                auto playerMapBlock = Space::MapBlock::world::fromBlock(client->getPos());

                std::pair<glm::ivec3, glm::ivec3> bounds = {
                        {playerMapBlock.x - MB_GEN_H, playerMapBlock.y - MB_GEN_V, playerMapBlock.z - MB_GEN_H},
                        {playerMapBlock.x + MB_GEN_H, playerMapBlock.y + MB_GEN_V, playerMapBlock.z + MB_GEN_H}};

                if (isInBounds(mapBlockPos, bounds) && client->getMapBlockIntegrity(mapBlockPos) < mapBlockIntegrity) {
                    client->setMapBlockIntegrity(mapBlockPos, mapBlockIntegrity);
                    sendChunk(chunk->pos, *client);
                }
            }
        }
    }

    // Send the # of generated chunks to the client (debug),
    // and trigger new chunks to be generated if a player has changed MapBlocks.
    Packet r(PacketType::SERVER_INFO);
    r.data = Serializer().append(generatedChunks).data;

    for (auto& client : clientList.clients) {
        if (client->hasPlayer) {
            r.sendTo(client->peer, PacketChannel::SERVER);

            if (client->changedMapBlocks) changedChunks(*client);
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

void ServerWorld::changedChunks(ServerClient& client) {
    auto mapBlock = Space::MapBlock::world::fromBlock(client.getPos());
    auto lastMapBlock = Space::MapBlock::world::fromBlock(client.lastPos);

    std::pair<glm::ivec3, glm::ivec3> oldBounds = {
            {lastMapBlock.x - MB_GEN_H, lastMapBlock.y - MB_GEN_V, lastMapBlock.z - MB_GEN_H},
            {lastMapBlock.x + MB_GEN_H, lastMapBlock.y + MB_GEN_V, lastMapBlock.z + MB_GEN_H}};

    unsigned int mapBlocksExisting = 0;
    unsigned int mapBlocksGenerating = 0;

    for (const auto &c : generateOrder) {
        glm::vec3 mapBlockPos = mapBlock + c;
        if (!isInBounds(mapBlockPos, oldBounds)) {
            auto existing = dimension.getMapBlock(mapBlockPos);
            if (existing != nullptr && existing->generated) {
                mapBlocksExisting++;
                sendMapBlock(mapBlockPos, client);
            }
            else {
                mapBlocksGenerating += generateMapBlock(mapBlockPos);
            }
        }
    }

    std::cout << "Generating " << mapBlocksGenerating << " blocks, sending " << mapBlocksExisting << " existing blocks." << std::endl;
    client.changedMapBlocks = false;
}

bool ServerWorld::generateMapBlock(glm::ivec3 pos) {
    if(!generateQueueMap.count(pos) && (!dimension.getMapBlock(pos) || !dimension.getMapBlock(pos)->generated)) {
        generateQueueMap.insert(pos);
        generateQueueList.push_back(pos);
        return true;
    }
    return false;
}

void ServerWorld::sendChunk(const std::shared_ptr<BlockChunk>& chunk, ServerClient &peer) {
    if (chunk == nullptr || !chunk->generated) return;

    Packet r = chunk->serialize();
    r.sendTo(peer.peer, PacketChannel::CHUNK);
}

void ServerWorld::sendChunk(const glm::ivec3& pos, ServerClient &peer) {
    sendChunk(dimension.getChunk(pos), peer);
}

void ServerWorld::sendMapBlock(const glm::ivec3& pos, ServerClient &peer) {
    unsigned long long mapBlockIntegrity = dimension.getMapBlockIntegrity(pos);
    if (peer.getMapBlockIntegrity(pos) < mapBlockIntegrity) {
        auto mapBlock = dimension.getMapBlock(pos);
        assert(mapBlock != nullptr);

        for (unsigned short i = 0; i < 64; i++) {
            if ((*mapBlock)[i] != nullptr) sendChunk((*mapBlock)[i], peer);
        }
    }
}

unsigned int ServerWorld::getBlock(glm::ivec3 pos) {
    return dimension.getBlock(pos);
}

void ServerWorld::setBlock(glm::ivec3 pos, unsigned int block) {
    auto oldBlock = getBlock(pos);

    if (block == DefinitionAtlas::AIR) {
        auto def = defs.defs.blockFromId(oldBlock);
        if (def.callbacks.count(Callback::DESTRUCT)) {
            def.callbacks[Callback::DESTRUCT](defs.parser.vecToTable(pos));
        }
    }
    else {
        auto def = defs.defs.blockFromId(block);
        if (def.callbacks.count(Callback::CONSTRUCT)) {
            def.callbacks[Callback::CONSTRUCT](defs.parser.vecToTable(pos));
        }
    }

    dimension.setBlock(pos, block);

    Packet b(PacketType::BLOCK_SET);
    b.data = Serializer()
            .append(pos)
            .append(block)
            .data;

    auto chunkPos = Space::Chunk::world::fromBlock(pos);

    for (auto &client : clientList.clients) {
        if (client->hasPlayer) {
            auto mapBlock = Space::MapBlock::world::fromBlock(client->getPos());

            std::pair<glm::ivec3, glm::ivec3> bounds = {
                    {mapBlock.x - MB_GEN_H, mapBlock.y - MB_GEN_V, mapBlock.z - MB_GEN_H},
                    {mapBlock.x + MB_GEN_H, mapBlock.y + MB_GEN_V, mapBlock.z + MB_GEN_H}};

            if (isInBounds(Space::MapBlock::world::fromChunk(chunkPos), bounds)) {
                b.sendTo(client->peer, PacketChannel::BLOCK);
            }
        }
    }

    if (block == DefinitionAtlas::AIR) {
        auto def = defs.defs.blockFromId(oldBlock);
        if (def.callbacks.count(Callback::AFTER_DESTRUCT)) {
            def.callbacks[Callback::AFTER_DESTRUCT](defs.parser.vecToTable(pos));
        }
    }
    else {
        auto def = defs.defs.blockFromId(block);
        if (def.callbacks.count(Callback::AFTER_CONSTRUCT)) {
            def.callbacks[Callback::AFTER_CONSTRUCT](defs.parser.vecToTable(pos));
        }
    }
}

bool ServerWorld::isInBounds(glm::ivec3 cPos, std::pair<glm::ivec3, glm::ivec3> &bounds) {
    return (cPos.x >= bounds.first.x && cPos.x <= bounds.second.x
         && cPos.y >= bounds.first.y && cPos.y <= bounds.second.y
         && cPos.z >= bounds.first.z && cPos.z <= bounds.second.z);
}