//
// Created by aurailus on 07/02/19.
//

#include "ConnectionList.h"
#include "../generic/network/PacketChannel.h"

ServerPeer* ConnectionList::addPeer(ENetPeer *eNetPeer) {
    printf("[INFO] %x:%u connected.\n", eNetPeer->address.host, eNetPeer->address.port);

    auto peer = new ServerPeer {.peer = eNetPeer, .player = nullptr, .index = peers.size()};
    eNetPeer->data = (void*)peer;

    peers.push_back(peer);
    return peer;
}

void ConnectionList::removePeer(ENetPeer *eNetPeer) {
    printf("[INFO] %x:%u disconnected.\n", eNetPeer->address.host, eNetPeer->address.port);

    auto peer = (ServerPeer*)eNetPeer->data;
    eNetPeer->data = nullptr;

    for (auto it = peers.begin(); it != peers.end(); it++) {
        if (peer == *it) {
            peers.erase(it);
            (*it)->cleanup();
            delete *it;
            break;
        }
    }
}

ServerPlayer* ConnectionList::addPlayer(ServerPeer *peer, std::string uuid) {
    printf("[INFO] Creating player %s for %x:%u.\n", uuid.c_str(), peer->peer->address.host, peer->peer->address.port);
    auto player = new ServerPlayer(peer);
    player->pos = glm::vec3(-8, 32, -8);

    //Send Initialization Data
    auto packet = player->getInitPacket();
    packet.sendTo(peer->peer, PacketChannel::PLAYERINFO);

    players.push_back(player);
}