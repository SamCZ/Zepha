//
// Created by aurailus on 22/01/19.
//

#include "ConnMan.h"
#include "../../generic/blocks/BlockChunk.h"

ConnMan::ConnMan() = default;

void ConnMan::init(unsigned short port) {
    server_socket = new asio::ip::udp::socket(io_context, udp::endpoint(udp::v4(), port));
}

void ConnMan::poll() {
    while (server_socket->available() > 0) {
        size_t pendingSize = server_socket->available();
        std::vector<Packet::PacketByte> recv_buf((unsigned long)pendingSize);

        auto remote_endpoint = new udp::endpoint();
        server_socket->receive_from(asio::buffer(recv_buf, pendingSize), *remote_endpoint);

        if (recv_buf.size() > 4) {
            auto packet = Packet::deserialize(recv_buf);
            handlePacket(packet, remote_endpoint);
        }
    }
}

void ConnMan::handlePacket(Packet* packet, udp::endpoint* endpoint) {
    std::string uuid = createIdentifier(endpoint);

    if (!clients.count(uuid)) {
        if (packet->type == Packet::HANDSHAKE) {

            auto client = addClient(uuid, endpoint);

            Packet p(Packet::HANDSHAKE);
            send(&p, client);
        }
    }
    else if (!clients[uuid]->authenticated) {
        if (packet->type == Packet::AUTHENTICATE) {
            int strLen = Packet::decodeInt(&packet->data[0]);
            int offset = 4;

            std::string token(packet->data.begin() + offset, packet->data.begin() + offset + strLen);

            //At some point there needs to be validation here with a centralized server, but for now
            //we just assume that the information is some sort of token and move on.

            offset = offset + strLen;
            strLen = Packet::decodeInt(&packet->data[offset]);
            offset += 4;

            std::string username(packet->data.begin() + offset, packet->data.begin() + offset + strLen);
            clients[uuid]->authenticated = true;
            addPlayer(clients[uuid], username);
        }
    }
    else {
        //TODO: Push to the packet vector
    }
}

ServerClient* ConnMan::addClient(std::string uuid, udp::endpoint* endpoint) {
    printf("[INFO] Handshake recieved from %s.\n", uuid.c_str());

    auto client = new ServerClient(endpoint, uuid);
    clients.insert(std::pair<std::string, ServerClient*>(uuid, client));
    return client;
}

void ConnMan::addPlayer(ServerClient* client, std::string username) {
    printf("[INFO] User %s has connected with IP %s.\n", username.c_str(), client->uuid.c_str());

    auto player = new ServerPlayer(client, glm::vec3(0, 64, 0));
    client->player = player;
    player->forceSendChunks = true;
    players.insert(std::pair<std::string, ServerPlayer*>(username, player));

    Packet p(Packet::PLAYERINFO);

    p.addFloat(player->pos.x);
    p.addFloat(player->pos.y);
    p.addFloat(player->pos.z);

    send(&p, client);
}

//Create a IP + Port combo that can be used as a UUID for an endpoint.
std::string ConnMan::createIdentifier(udp::endpoint *endpoint) {
    return endpoint->address().to_string() + ":" + std::to_string(endpoint->port());
}

void ConnMan::send(Packet *p, ServerClient* client) {
    send(p, client->endpoint);
}

void ConnMan::send(Packet *p, udp::endpoint* endpoint) {
    auto data = p->serialize();
    server_socket->send_to(asio::buffer(data, data.size()), *endpoint);
    std::cout << "sent" << std::endl;
}

std::map<std::string, ServerPlayer*>* ConnMan::getPlayersMap() {
    return &players;
}

ConnMan::~ConnMan() {
    delete server_socket;
}
