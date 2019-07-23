//
// Created by aurailus on 29/06/19.
//

#include <iostream>
#include "ServerClient.h"

ServerClient::ServerClient(ENetPeer *peer, ENetAddress address) :
    peer(peer),
    address(address),
    connectID(peer->connectID) {
}

unsigned int ServerClient::getConnectID() {
    return connectID;
}

void ServerClient::initPlayer() {
    //TODO: IMPLEMENT USERNAME
    delete player;
    player = new ServerPlayer({0, 16, 0}, connectID, "TEMPORARY");
}

bool ServerClient::hasPlayer() {
    return player != nullptr;
}

ServerPlayer& ServerClient::getPlayer() {
    if (!player) throw "getPlayer() called on client without player!";
    return *player;
}

ENetPeer& ServerClient::getPeer() {
    return *peer;
}

ServerClient::~ServerClient() {
    delete player;
}
