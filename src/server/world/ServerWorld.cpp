//
// Created by aurailus on 05/03/19.
//

#include <algorithm>
#include <glm/glm.hpp>
#include "ServerWorld.h"
#include "../../util/net/PacketChannel.h"
#include "../../util/Timer.h"
#include "../../util/Util.h"

const static int MB_GEN_H = 3;
const static int MB_GEN_V = 3;

ServerWorld::ServerWorld(unsigned int seed, ServerDefs& defs, ServerClients& clients) :
    seed(seed),
    defs(defs),
    clientList(clients) {

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
    delete genStream;
    genStream = new WorldGenStream(seed, defs);
}

void ServerWorld::changedChunks(ServerClient& client) {
    auto mapBlock = client.getPlayer().mapBlock;
    auto lastMapBlock = client.getPlayer().lastMapBlock;

    std::pair<glm::vec3, glm::vec3> oldBounds = {
            {lastMapBlock.x - MB_GEN_H, lastMapBlock.y - MB_GEN_V, lastMapBlock.z - MB_GEN_H},
            {lastMapBlock.x + MB_GEN_H, lastMapBlock.y + MB_GEN_V, lastMapBlock.z + MB_GEN_H}};

    std::cout << "Attempting to generate" << std::endl;
    for (const auto &c : generateOrder) {
        glm::vec3 mapBlockPos = mapBlock + c;
        if (!isInBounds(mapBlockPos, oldBounds)) {
            //TODO: This
//            if (dimension.getMapBlock(mapBlockPos) != nullptr) {
//                sendChunk(chunkPos, client);
//            }
//            else {
                generateMapBlock(c + mapBlock);
//            }
        }
    }
    client.getPlayer().changedMapBlocks = false;
}

void ServerWorld::generateMapBlock(glm::vec3 pos) {
    if(!generateQueueMap.count(pos) && !dimension.getChunk(pos)) {
        generateQueueMap.insert(pos);
        generateQueueList.push_back(pos);
    }
}

void ServerWorld::update() {
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
    generatedChunks = (int)finished.size();

    for (const auto& chunk : finished) {
        dimension.setChunk(chunk);

        for (auto& client : clientList.clients) {
            if (client->hasPlayer()) {
                auto mapBlock = client->getPlayer().mapBlock;

                std::pair<glm::vec3, glm::vec3> bounds = {
                        {mapBlock.x - MB_GEN_H, mapBlock.y - MB_GEN_V, mapBlock.z - MB_GEN_H},
                        {mapBlock.x + MB_GEN_H, mapBlock.y + MB_GEN_V, mapBlock.z + MB_GEN_H}};

                if (isInBounds(Space::MapBlock::world::fromChunk(chunk->pos), bounds)) {
                    sendChunk(chunk->pos, *client);
                }
            }
        }
    }

    Packet r(PacketType::SERVER_INFO);
    Serializer::encodeInt(r.data, generatedChunks);

    for (auto& client : clientList.clients) {
        if (client->hasPlayer()) {
            r.sendTo(client->getPeer(), PacketChannel::SERVER);

            if (client->getPlayer().changedMapBlocks) changedChunks(*client);
        }
    }
}

void ServerWorld::sendChunk(glm::vec3 pos, ServerClient &peer) {
    auto chunk = dimension.getChunk(pos);
    assert(chunk != nullptr);
    auto serialized = chunk->serialize();

    Packet r(PacketType::CHUNK);

    Serializer::encodeIntVec3(r.data, pos);
    Serializer::encodeString(r.data, serialized);

    r.sendTo(peer.getPeer(), PacketChannel::CHUNK);
}

void ServerWorld::setBlock(glm::vec3 pos, unsigned int block) {
    auto oldBlock = getBlock(pos);

    if (block == DefinitionAtlas::AIR) {
        auto def = defs.defs().blockFromId(oldBlock);
        if (def.callbacks.count(Callback::DESTRUCT)) {
            def.callbacks[Callback::DESTRUCT](defs.lua().vecToTable(pos));
        }
    }
    else {
        auto def = defs.defs().blockFromId(block);
        if (def.callbacks.count(Callback::CONSTRUCT)) {
            def.callbacks[Callback::CONSTRUCT](defs.lua().vecToTable(pos));
        }
    }

    dimension.setBlock(pos, block);

    Packet b(PacketType::BLOCK_SET);
    Serializer::encodeIntVec3(b.data, pos);
    Serializer::encodeInt(b.data, block);

    auto chunkPos = Space::Chunk::world::fromBlock(pos);

    for (auto &client : clientList.clients) {
        if (client->hasPlayer()) {
            auto mapBlock = client->getPlayer().mapBlock;

            std::pair<glm::vec3, glm::vec3> bounds = {
                    {mapBlock.x - MB_GEN_H, mapBlock.y - MB_GEN_V, mapBlock.z - MB_GEN_H},
                    {mapBlock.x + MB_GEN_H, mapBlock.y + MB_GEN_V, mapBlock.z + MB_GEN_H}};

            if (isInBounds(Space::MapBlock::world::fromChunk(chunkPos), bounds)) {
                b.sendTo(client->getPeer(), PacketChannel::BLOCK);
            }
        }
    }

    if (block == DefinitionAtlas::AIR) {
        auto def = defs.defs().blockFromId(oldBlock);
        if (def.callbacks.count(Callback::AFTER_DESTRUCT)) {
            def.callbacks[Callback::AFTER_DESTRUCT](defs.lua().vecToTable(pos));
        }
    }
    else {
        auto def = defs.defs().blockFromId(block);
        if (def.callbacks.count(Callback::AFTER_CONSTRUCT)) {
            def.callbacks[Callback::AFTER_CONSTRUCT](defs.lua().vecToTable(pos));
        }
    }
}

bool ServerWorld::isInBounds(glm::vec3 cPos, std::pair<glm::vec3, glm::vec3> &bounds) {
    return (cPos.x >= bounds.first.x && cPos.x <= bounds.second.x
         && cPos.y >= bounds.first.y && cPos.y <= bounds.second.y
         && cPos.z >= bounds.first.z && cPos.z <= bounds.second.z);
}

unsigned int ServerWorld::getBlock(glm::vec3 pos) {
    return dimension.getBlock(pos);
}

ServerWorld::~ServerWorld() {
    delete genStream;
}
