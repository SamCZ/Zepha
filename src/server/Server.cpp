//
// Created by aurailus on 09/01/19.
//

#include "Server.h"
#include "../generic/blocks/BlockChunk.h"

Server::Server() = default;

Server::Server(unsigned short port) {
    this->port = port;
}

void Server::init() {
    conn.init(port);
    players = conn.getPlayersMap();

    while (alive) loop();
}

void Server::loop() {
    Timer loop("");

    conn.poll();

    for (auto &p : *players) {
        if (p.second->forceSendChunks) {
            sendChunks(*p.second);
            p.second->forceSendChunks = false;
        }
    }

    long sleep_for = 16L*1000000L - loop.elapsedNs();
    std::this_thread::sleep_for(std::chrono::nanoseconds(sleep_for));
}

void Server::sendChunks(ServerPlayer &player) {
    const int SEND_RANGE = 10;

    auto blocks = new std::vector<int>(4096);
    for (int i = 0; i < 4096; i++) {
        (*blocks)[i] = (int)round(rand() % 5);
    }
    auto chunk = new BlockChunk(blocks);
    auto chunkString = chunk->serialize();

    for (int i = -SEND_RANGE; i < SEND_RANGE; i++) {
        for (int j = -SEND_RANGE; j < SEND_RANGE; j++) {
            for (int k = -SEND_RANGE; k < SEND_RANGE; k++) {
                Packet p(Packet::CHUNKINFO);

                p.addInt(i);
                p.addInt(j);
                p.addInt(k);

//                p.addString(chunkString);

                conn.send(&p, player.connection);
            }
        }
    }
}

void Server::cleanup() {
    alive = false;
}

Server::~Server() {
    cleanup();
}