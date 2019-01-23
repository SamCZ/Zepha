//
// Created by aurailus on 09/01/19.
//

#include "Server.h"

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

    long sleep_for = 16L*1000000L - loop.elapsedNs();
    std::this_thread::sleep_for(std::chrono::nanoseconds(sleep_for));
}

void Server::cleanup() {
    alive = false;
}

Server::~Server() {
    cleanup();
}
