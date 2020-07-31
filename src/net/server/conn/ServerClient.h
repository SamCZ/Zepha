//
// Created by aurailus on 2020-07-30.
//

#pragma once

#include <string>
#include <memory>
#include <enet/enet.h>

class ServerPlayer;

class ServerClient {
public:
    ServerClient(const std::string& username, ENetPeer* peer) :
        username(username), peer(peer), address(peer->address), id(peer->connectID) {}

    std::string username;
    unsigned int id = 0;

    ENetPeer* peer;
    ENetAddress address;

    std::shared_ptr<ServerPlayer> player = nullptr;
};
