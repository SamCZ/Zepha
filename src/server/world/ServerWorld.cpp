//
// Created by aurailus on 05/03/19.
//

#include <algorithm>
#include <glm.hpp>
#include "ServerWorld.h"
#include "../../util/net/PacketChannel.h"
#include "../../util/Timer.h"
#include "../../util/Util.h"

ServerWorld::ServerWorld(unsigned int seed) : genStream(seed) {
    //Pregenerate chunk generation order
    generateOrder.reserve((unsigned long)pow(ServerPlayer::ACTIVE_RANGE_H * 2, 3));


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

    std::cout << "Generated Chunk Queue is " << generateOrder.size() << " chunks long." << std::endl;
}

void ServerWorld::addPlayer(ServerPlayer *player) {
    Timer t("New Chunk Allocation");

    this->players.push_back(player);

    auto bounds = player->getBounds();
    auto pos = player->getChunkPos();

    for (const auto &c : generateOrder) {
        glm::vec3 chunkPos = {c.x + pos.x, c.y + pos.y, c.z + pos.z};
        if (dimension.getChunk(chunkPos) != nullptr) {
            sendChunk(chunkPos, *player->peer);
        }
        else {
            generate(chunkPos);
        }
    }

    t.printElapsedMs();
}

void ServerWorld::playerChangedChunks(ServerPlayer *player) {
    Timer t("[INFO] Movement Allocation");

    auto pos = player->getChunkPos();

    auto bounds = player->getBounds();
    auto oldBounds = player->getOldBounds();

    int generated = 0;

    for (const auto &c : generateOrder) {
        glm::vec3 chunkPos = {c.x + pos.x, c.y + pos.y, c.z + pos.z};
        if (!player->isInBounds(chunkPos, oldBounds)) {
            if (dimension.getChunk(chunkPos) != nullptr) {
                sendChunk(chunkPos, *player->peer);
            }
            else {
                generate(chunkPos);
                generated++;
            }
        }
    }

    printf("[INFO] %s moved to %d, %d, %d, generating %d chunks.\n",
           player->getUsername().c_str(),
           static_cast<int>(pos.x),
           static_cast<int>(pos.y),
           static_cast<int>(pos.z),
           generated);

    t.printElapsedMs();

    player->changedChunks = false;
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

        if (genStream.tryToQueue(pos)) {
            generateQueueList.erase(it);
            generateQueueMap.erase(pos);
        }
        else break;
    }

    auto finished = genStream.update();
    generatedChunks = (int)finished.size();

    for (const auto &chunk : finished) {
        dimension.addBlockChunk(chunk);

        for (auto player : players) {
            auto bounds = player->getBounds();

            if (player->isInBounds(chunk->pos, bounds)) {
                sendChunk(chunk->pos, *player->peer);
            }
        }
    }

    Packet r(Packet::SERVER_INFO);
    Serializer::encodeInt(r.data, generatedChunks);

    for (auto player : players) {
        r.sendTo(player->peer->peer, PacketChannel::SERVER_INFO);

        //Run update method for players
        if (player->changedChunks) {
            playerChangedChunks(player);
        }
    }
}

void ServerWorld::sendChunk(glm::vec3 pos, ServerPeer &peer) {
    ///TODO: Remove null check once bug is fixed
    auto chunk = dimension.getChunk(pos);
    if (chunk != nullptr) {
        auto serialized = chunk->serialize();

        Packet r(Packet::CHUNK_INFO);

        Serializer::encodeIntVec3(r.data, pos);
        Serializer::encodeString(r.data, serialized);

        r.sendTo(peer.peer, PacketChannel::CHUNKS);
    }
    else {
        //BUG: If the null chunks are on the client, The Null chunks have to happen there, cause the chunks are serialized before sending.
        std::cerr << "Tried to send null chunk at " << pos.x << ", " << pos.y << ", " << pos.z << std::endl;
    }
}

void ServerWorld::setBlock(glm::vec3 pos, int block) {
    dimension.setBlock(pos, block);

    Packet b(Packet::BLOCK_SET);
    Serializer::encodeIntVec3(b.data, pos);
    Serializer::encodeInt(b.data, block);

    auto chunkPos = TransPos::chunkFromVec(TransPos::roundPos(pos));

    for (auto player : players) {
        auto bounds = player->getBounds();

        if (player->isInBounds(chunkPos, bounds)) {
            b.sendTo(player->peer->peer, PacketChannel::BLOCK_UPDATES);
        }
    }
}