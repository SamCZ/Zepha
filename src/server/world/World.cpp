//
// Created by aurailus on 05/03/19.
//

#include <algorithm>
#include <glm.hpp>
#include "World.h"
#include "../../generic/network/PacketChannel.h"
#include "../../client/engine/Timer.h"

void World::addPlayer(ServerPlayer *player) {
    Timer t("New Chunk Allocation");

    this->players.push_back(player);

    auto bounds = player->getBounds();
    auto pos = player->getChunkPos();

    std::vector<glm::vec3> toGenerate;
    toGenerate.reserve((unsigned long)pow(ServerPlayer::ACTIVE_RANGE, 3));

    for (int i = (int)bounds.first.x; i <= (int)bounds.second.x; i++) {
        for (int j = (int)bounds.first.y; j <= (int)bounds.second.y; j++) {
            for (int k = (int) bounds.first.z; k <= (int) bounds.second.z; k++) {
                toGenerate.emplace_back(i, j, k);
            }
        }
    }

    std::sort(toGenerate.begin(), toGenerate.end(), [&](glm::vec3 a, glm::vec3 b) {
        using namespace std;
        return max(max(abs(a.x - pos.x), abs(a.y - pos.y)), abs(a.z - pos.z)) <
               max(max(abs(b.x - pos.x), abs(b.y - pos.y)), abs(b.z - pos.z));
    });

    for (glm::vec3 tPos : toGenerate) {
        generate(tPos);
    }

    t.printElapsedMs();
}

void World::playerChangedChunks(ServerPlayer *player) {
    Timer t("Movement Allocation");

    auto pos = player->getChunkPos();

    auto bounds = player->getBounds();
    auto oldBounds = player->getOldBounds();

    std::vector<glm::vec3> toGenerate;
    toGenerate.reserve((unsigned long)pow(ServerPlayer::ACTIVE_RANGE, 3));

    for (int i = (int)bounds.first.x; i <= (int)bounds.second.x; i++) {
        for (int j = (int)bounds.first.y; j <= (int)bounds.second.y; j++) {
            for (int k = (int) bounds.first.z; k <= (int) bounds.second.z; k++) {
                glm::vec3 tPos(i, j, k);
                if (!player->isInBounds(tPos, oldBounds)) {
                    toGenerate.push_back(tPos);
                }
            }
        }
    }

    std::sort(toGenerate.begin(), toGenerate.end(), [&](glm::vec3 a, glm::vec3 b) {
        using namespace std;
        return max(max(abs(a.x - pos.x), abs(a.y - pos.y)), abs(a.z - pos.z)) <
               max(max(abs(b.x - pos.x), abs(b.y - pos.y)), abs(b.z - pos.z));
    });

    for (glm::vec3 tPos : toGenerate) {
        generate(tPos);
    }

    printf("[INFO] %s moved, generating %d chunks.\n",
            player->getUsername().c_str(), (int)toGenerate.size());
    t.printElapsedMs();

    player->changedChunks = false;
}

void World::generate(glm::vec3 pos) {
    if(!generateQueueMap.count(pos) && !chunkMap.count(pos)) {

        generateQueueMap.insert(pos);
        generateQueueList.push_back(pos);
    }
}

void World::update() {
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

    for (auto chunk : finished) {

        bool didCalcSerialized = false;
        std::string serialized;

        for (auto player : players) {
            auto bounds = player->getBounds();
            if (player->isInBounds(chunk->pos, bounds)) {

                //Serialize the chunk
                if (!didCalcSerialized) {
                    serialized = chunk->serialize();
                    didCalcSerialized = true;
                }

                //Send the Chunk to the player
                Packet r(Packet::CHUNK_INFO);

                Serializer::encodeInt(r.data, (int)chunk->pos.x);
                Serializer::encodeInt(r.data, (int)chunk->pos.y);
                Serializer::encodeInt(r.data, (int)chunk->pos.z);
                Serializer::encodeString(r.data, serialized);

                r.sendTo(player->peer->peer, PacketChannel::WORLD_INFO);
            }
        }
    }

    Packet r(Packet::SERVER_INFO);
    Serializer::encodeInt(r.data, generatedChunks);

    for (auto player : players) {
        r.sendTo(player->peer->peer, PacketChannel::SERVER_INFO);

        if (player->changedChunks) {
            playerChangedChunks(player);
        }
    }
}