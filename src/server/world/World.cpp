//
// Created by aurailus on 05/03/19.
//

#include "World.h"
#include "../../generic/network/PacketChannel.h"

void World::addPlayer(ServerPlayer *player) {
    this->players.push_back(player);

    playerChangedChunks(player);
}

void World::playerChangedChunks(ServerPlayer *player) {
    auto bounds = player->getBounds();

    for (auto i = (int)bounds.first.x; i < (int)bounds.second.x; i++) {
        for (auto j = (int)bounds.first.y; j < (int)bounds.second.y; j++) {
            for (auto k = (int)bounds.first.z; k < (int)bounds.second.z; k++) {
                generate(glm::vec3(i, j, k));
            }
        }
    }
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

        bool success = genStream.tryToQueue(pos);

        if (success) {
            generateQueueList.erase(it);
            generateQueueMap.erase(pos);
        }
        else break;
    }

    auto finished = genStream.update();
    for (auto chunk : finished) {

        bool didCalcSerialized = false;
        std::string serialized;

        for (auto player : players) {
            if (player->isInBounds(chunk->pos)) {

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
}