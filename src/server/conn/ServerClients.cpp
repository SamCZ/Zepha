//
// Created by aurailus on 07/02/19.
//

#include "ServerClients.h"
#include "../../util/net/Serializer.h"

void ServerClients::handleConnect(ENetEvent e) {
    ENetPeer* peer = e.peer;
    ENetAddress& addr = peer->address;

    std::cout << Log::info << NetHandler::intToIPString(addr.host) << ":" << addr.port << " connected." << Log::endl;

    auto client = new ServerClient(peer, addr);
    clients.push_back(client);
    peer->data = client;
}

void ServerClients::handleDisconnect(ENetEvent e) {
    ENetPeer* peer = e.peer;
    ENetAddress& addr = peer->address;

    unsigned int cid = static_cast<ServerClient*>(peer->data)->cid;

    std::cout << Log::info << NetHandler::intToIPString(addr.host) << ":" << addr.port << " disconnected." << Log::endl;

    bool found = false;
    auto it = clients.begin();
    while (it != clients.end()) {
        if ((*it)->cid == cid) {
            found = true;
            clients.erase(it);
            break;
        }
        it ++;
    }

    if (!found) std::cout << Log::err << "Tried to disconnect nonexistent client!" << Log::endl;
}

void ServerClients::createPlayer(ServerClient &c) {
    c.hasPlayer = true;

    Packet p(PacketType::THIS_PLAYER_INFO);
    p.data = Serializer()
            .append(c.cid)
            .append(c.getPos())
            .append(c.getAngle())
            .data;

    p.sendTo(c.peer, PacketChannel::ENTITY);
}
