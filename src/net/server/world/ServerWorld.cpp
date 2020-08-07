//
// Created by aurailus on 05/03/19.
//

#include <iostream>
#include <glm/glm.hpp>
#include <unordered_map>

#include "ServerWorld.h"

#include "ServerLuaEntity.h"
#include "ServerGenStream.h"
#include "../../Serializer.h"
#include "ServerPacketStream.h"
#include "../conn/ServerPlayer.h"
#include "../conn/ServerClients.h"
#include "../../../def/item/BlockDef.h"
#include "../../../world/chunk/Chunk.h"
#include "../../../lua/usertype/Target.h"
#include "../../../world/chunk/MapBlock.h"
#include "../../../game/inventory/ServerInventoryRefs.h"

ServerWorld::ServerWorld(unsigned int seed, SubgamePtr game, ServerClients& clients) :
    World(game),
    seed(seed),
    clients(clients),
    refs(std::make_shared<ServerInventoryRefs>(game, clients)) {

    clients.init(this);

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
    genStream = std::make_unique<ServerGenStream>(*game.s(), *this, seed);
    packetStream = std::make_unique<ServerPacketStream>(*this);
//    fileManip = std::make_shared<FileManipulator>("worlds/" + worldDir + "/");
}

void ServerWorld::update(double delta) {
    World::update(delta);
    refs->update();

    unsigned int genCount = 0;
    std::unordered_set<glm::ivec4, Vec::ivec4> updatedChunks {};

    auto finishedGen = genStream->update();
    for (auto& data : *finishedGen) {
        for (const auto& pos : *data.created)
            updatedChunks.insert(glm::ivec4(pos, data.dim));

        getDimension(data.dim)->getMapBlock(glm::ivec3(data.pos))->generated = true;
        packetStream->queue(data.dim, data.pos);
        genCount++;
    }

    auto finishedPackets = packetStream->update();
    for (auto& data : *finishedPackets) {
        for (auto& player : clients.players)
            data->packet->sendTo(player->getPeer(), Packet::Channel::WORLD);
    }

    this->generatedMapBlocks = genCount;

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
//        l.unlock();
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

    Packet r = Serializer().append(this->generatedMapBlocks).packet(Packet::Type::SERVER_INFO);

    for (auto& player : clients.players) {
        r.sendTo(player->getPeer(), Packet::Channel::SERVER);
        if (player->changedMapBlocks) changedMapBlocks(*player);
    }

    for (auto& d : dimensions) {
        auto dimension = std::static_pointer_cast<ServerDimension>(d);
        unsigned int ind = dimension->getInd();

        // Update clients with new entity information.

        Serializer inf;
        inf.append(ind);

        for (auto& entity : dimension->getLuaEntities()) {
            auto opt = entity.entity.s()->serialize();
            if (opt) inf.append(*opt);
        }

        if (inf.data.size() > 4) {
            auto p = inf.packet(Packet::Type::ENTITY_INFO);
            for (auto& player : clients.players)
                if (player->getDim()->getInd() == ind)
                    p.sendTo(player->getPeer(), Packet::Channel::ENTITY);
        }

        // Update clients with removed entities.

        Serializer rem;
        rem.append(ind);

        for (unsigned int entity : dimension->getRemovedEntities()) {
            rem.append<unsigned int>(entity);
        }

        if (rem.data.size() > 4) {
            Packet p = rem.packet(Packet::Type::ENTITY_REMOVED);
            for (auto& player : clients.players)
                if (player->getDim()->getInd() == ind)
                    p.sendTo(player->getPeer(), Packet::Channel::ENTITY);
        }

        dimension->clearRemovedEntities();
    }
}

DimensionPtr ServerWorld::createDimension(const std::string &identifier) {
    this->dimensions.emplace_back(std::make_shared<ServerDimension>(game, *this, identifier, this->dimensions.size()));
    return dimensions[dimensions.size() - 1];
}

DimensionPtr ServerWorld::getDimension(unsigned int index) {
    return dimensions[index];
}

DimensionPtr ServerWorld::getDimension(const std::string &identifier) {
    for (auto& dimension : dimensions)
        if (dimension->getIdentifier() == identifier) return dimension;
    throw std::runtime_error("No dimension named " + identifier + " found.");
}

InventoryRefsPtr ServerWorld::getRefs() {
    return InventoryRefsPtr(refs);
}

void ServerWorld::changedMapBlocks(ServerPlayer& player) {
    generateMapBlocks(player);
    sendChunksToPlayer(player);
    player.changedMapBlocks = false;
}

void ServerWorld::generateMapBlocks(ServerPlayer& player) {
    unsigned int generating = 0;
    glm::ivec3 playerMapBlock = Space::MapBlock::world::fromBlock(player.getPos());

    for (const auto &c : generateOrder) {
        glm::ivec3 mapBlockPos = playerMapBlock + c;
        auto existing = player.getDim()->getMapBlock(mapBlockPos);
        if (existing && existing->generated) continue;
        else generating += generateMapBlock(player.getDim()->getInd(), mapBlockPos);
    }

    std::cout << "Player moved, generating " << generating << " MapBlocks." << std::endl;
}

bool ServerWorld::generateMapBlock(unsigned int dim, glm::ivec3 pos) {
    auto dimension = getDimension(dim);
    if(!dimension->getMapBlock(pos) || !dimension->getMapBlock(pos)->generated) return genStream->queue(dim, pos);
    return false;
}

void ServerWorld::sendChunksToPlayer(ServerPlayer& client) {
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
                packetStream->queue(client.getDim()->getInd(), pos);
            }
        }
    }
}

bool ServerWorld::isInBounds(glm::ivec3 cPos, std::pair<glm::ivec3, glm::ivec3> &bounds) {
    return (cPos.x >= bounds.first.x && cPos.x <= bounds.second.x
         && cPos.y >= bounds.first.y && cPos.y <= bounds.second.y
         && cPos.z >= bounds.first.z && cPos.z <= bounds.second.z);
}