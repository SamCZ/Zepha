//
// Created by aurailus on 07/02/19.
//

#include "ConnectionList.h"
#include "../../util/net/PacketChannel.h"

ConnectionList::ConnectionList(ServerWorld* world) {
    this->world = world;
}

ServerPeer* ConnectionList::addPeer(ENetPeer *eNetPeer) {
    std::cout << Log::info << eNetPeer->address.host << ":" << eNetPeer->address.port << " connected." << Log::endl;

    auto peer = new ServerPeer {.peer = eNetPeer, .player = nullptr, .index = (int)peers.size()};
    eNetPeer->data = (void*)peer;

    peers.push_back(peer);
    return peer;
}

void ConnectionList::removePeer(ENetPeer *eNetPeer) {
    std::cout << Log::info << eNetPeer->address.host << ":" << eNetPeer->address.port << " disconnected." << Log::endl;

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

ServerPlayer* ConnectionList::createPlayer(ServerPeer *peer, std::string uuid, std::string username) {
    std::cout << Log::info << "Creating player " << uuid << " for "
              << peer->peer->address.host << ":" << peer->peer->address.port << "." << Log::endl;

    auto player = new ServerPlayer(peer, uuid, std::move(username));
    player->setPos(glm::vec3(0, 16, 0));

    //Send Initialization Data
    auto packet = player->getInitPacket();
    packet.sendTo(peer->peer, PacketChannel::PLAYER_INFO);

    world->addPlayer(player);

    return player;
}