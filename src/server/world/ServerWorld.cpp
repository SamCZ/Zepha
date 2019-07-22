//
// Created by aurailus on 05/03/19.
//

#include <algorithm>
#include <glm/glm.hpp>
#include "ServerWorld.h"
#include "../../util/net/PacketChannel.h"
#include "../../util/Timer.h"
#include "../../util/Util.h"

ServerWorld::ServerWorld(unsigned int seed, ServerDefs& defs, ServerClients& clients) :
    seed(seed),
    defs(defs),
    clientList(clients) {

    //Pregenerate chunk generation order
    generateOrder.reserve((unsigned long)pow(ServerPlayer::ACTIVE_RANGE_H * 2, 2) * ServerPlayer::ACTIVE_RANGE_V * 2);

    for (int i = 0; i <= ServerPlayer::ACTIVE_RANGE_H; i++) {
        for (int j = 0; j <= i; j++) {
            for (int k = -ServerPlayer::ACTIVE_RANGE_V; k <= ServerPlayer::ACTIVE_RANGE_V; k++) {
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
    genStream = new WorldGenStream(seed, defs.blocks());
}

void ServerWorld::changedChunks(ServerClient& client) {
    auto pos = client.getPlayer().getChunkPos();

    auto bounds = client.getPlayer().getChunkBounds();
    auto oldBounds = client.getPlayer().getLastChunkBounds();

    int generated = 0;

    for (const auto &c : generateOrder) {
        glm::vec3 chunkPos = {c.x + pos.x, c.y + pos.y, c.z + pos.z};
        if (!isInBounds(chunkPos, oldBounds)) {
            if (dimension.getChunk(chunkPos) != nullptr) {
                sendChunk(chunkPos, client);
            }
            else {
                generate(chunkPos);
                generated++;
            }
        }
    }

    client.getPlayer().changedChunks = false;
}

void ServerWorld::generate(glm::vec3 pos) {
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
        dimension.addBlockChunk(chunk);

        for (auto& client : clientList.clients) {
            if (client.hasPlayer()) {
                auto bounds = client.getPlayer().getChunkBounds();

                if (isInBounds(chunk->pos, bounds)) {
                    sendChunk(chunk->pos, client);
                }
            }
        }
    }

    Packet r(PacketType::SERVER_INFO);
    Serializer::encodeInt(r.data, generatedChunks);

    for (auto& client : clientList.clients) {
        if (client.hasPlayer()) {
            r.sendTo(client.getPeer(), PacketChannel::SERVER);

            if (client.getPlayer().changedChunks) changedChunks(client);
        }
    }
}

void ServerWorld::sendChunk(glm::vec3 pos, ServerClient &peer) {
    auto chunk = dimension.getChunk(pos);
    auto serialized = chunk->serialize();

    Packet r(PacketType::CHUNK);

    Serializer::encodeIntVec3(r.data, pos);
    Serializer::encodeString(r.data, serialized);

    r.sendTo(peer.getPeer(), PacketChannel::CHUNK);
}

void ServerWorld::setBlock(glm::vec3 pos, int block) {
    auto oldBlock = getBlock(pos);

    if (block == 0) {
        auto def = defs.blocks().fromIndex(oldBlock);
        if (def.callbacks.count(Callback::DESTRUCT)) {
            def.callbacks[Callback::DESTRUCT](defs.lua().vecToTable(pos));
        }
    }
    else {
        auto def = defs.blocks().fromIndex(block);
        if (def.callbacks.count(Callback::CONSTRUCT)) {
            def.callbacks[Callback::CONSTRUCT](defs.lua().vecToTable(pos));
        }
    }

    dimension.setBlock(pos, block);

    Packet b(PacketType::BLOCK_SET);
    Serializer::encodeIntVec3(b.data, pos);
    Serializer::encodeInt(b.data, block);

    auto chunkPos = TransPos::chunkFromVec(TransPos::roundPos(pos));

    for (auto &client : clientList.clients) {
        if (client.hasPlayer()) {
            auto bounds = client.getPlayer().getChunkBounds();

            if (isInBounds(chunkPos, bounds)) {
                b.sendTo(client.getPeer(), PacketChannel::BLOCK);
            }
        }
    }

    if (block == 0) {
        auto def = defs.blocks().fromIndex(oldBlock);
        if (def.callbacks.count(Callback::AFTER_DESTRUCT)) {
            def.callbacks[Callback::AFTER_DESTRUCT](defs.lua().vecToTable(pos));
        }
    }
    else {
        auto def = defs.blocks().fromIndex(block);
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

int ServerWorld::getBlock(glm::vec3 pos) {
    return dimension.getBlock(pos);
}

ServerWorld::~ServerWorld() {
    delete genStream;
}
