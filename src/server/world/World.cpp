//
// Created by aurailus on 05/03/19.
//

#include "World.h"
#include "../../generic/network/PacketChannel.h"

void World::addPlayer(ServerPlayer *player) {
    this->players.push_back(player);

    auto bounds = player->getBounds();
    auto pos = player->getChunkPos();

    std::vector<glm::vec3> toGenerate;
    toGenerate.reserve((unsigned long)pow(ServerPlayer::ACTIVE_RANGE, 3));

    for (int i = (int)bounds.first.x; i < (int)bounds.second.x; i++) {
        for (int j = (int)bounds.first.x; j < (int)bounds.second.x; j++) {
            for (int k = (int) bounds.first.x; k < (int) bounds.second.x; k++) {
                toGenerate.emplace_back(i, j, k);
            }
        }
    }

    std::sort(toGenerate.begin(), toGenerate.end(), [&](glm::vec3 a, glm::vec3 b) {
        return max(max(abs(a.x - pos.x), abs(a.y - pos.y)), abs(a.z - pos.z)) <
               max(max(abs(b.x - pos.x), abs(b.y - pos.y)), abs(b.z - pos.z));
    });

    for (glm::vec3 tPos : toGenerate) {
        generate(tPos);
    }
}

void World::playerChangedChunks(ServerPlayer *player) {
    auto pos = player->getChunkPos();
    auto oldBounds = player->getOldBounds();

    //TODO: Generate chunks when the player moves

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