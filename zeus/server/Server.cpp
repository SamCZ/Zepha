//
// Created by aurailus on 09/01/19.
//

#include "Server.h"
#include "../game/world/Player.h"

Server::Server() = default;

Server::Server(int port) {
    this->port = port;
    alive = true;
}

void Server::start() {
    server_socket = new asio::ip::udp::socket(io_context, udp::endpoint(udp::v4(), (unsigned short)port));

    while (alive) loop();
}

void Server::loop() {
    Timer t("Loop time");

    //Collect incoming packets
    while (server_socket->available() > 0) {
        size_t pendingSize = server_socket->available();
        std::vector<Packet::PacketByte> recv_buf((unsigned long)pendingSize);

        auto remote_endpoint = new udp::endpoint();
        server_socket->receive_from(asio::buffer(recv_buf, pendingSize), *remote_endpoint);

        auto packet = Packet::deserialize(recv_buf);
        if (packet.length > 0) handlePacket(packet, remote_endpoint);
    }

    long sleep_for = 16L*1000000L - t.elapsedNs();
    std::this_thread::sleep_for(std::chrono::nanoseconds(sleep_for));
}

std::string Server::createIdentifier(udp::endpoint *endpoint) {
    return endpoint->address().to_string() + ":" + std::to_string(endpoint->port());
}

void Server::handlePacket(Packet &packet, udp::endpoint* endpoint) {
    auto identifier = createIdentifier(endpoint);

    if (connections.count(identifier) == 0 || !connections[identifier]->authenticated) {
        handleAuthPacket(identifier, packet, endpoint);
        return;
    }

    std::cout << packet.type << ", " << Packet::decodeInt(&packet.data[0]) << std::endl;
}

void Server::handleAuthPacket(std::string& identifier, Packet &packet, udp::endpoint* endpoint) {
    if (connections.count(identifier) == 0) {
        if (packet.type == Packet::HANDSHAKE) {
            addConnection(identifier, endpoint);

            Packet p;
            p = Packet(Packet::HANDSHAKE);
            auto data = p.serialize();

            server_socket->send_to(asio::buffer(data, data.size()), *endpoint);
            return;
        }
    }
    else {
        if (packet.type == Packet::AUTHTOKEN) {
            int strLen = Packet::decodeInt(&packet.data[0]);

            std::string token(packet.data.begin() + 4, packet.data.begin() + 4 + strLen);

            //TODO: Validate this token somehow
            std::cout << "Token: " << token << std::endl;

            createPlayer(connections[identifier]);
            return;
        }
    }
}

void Server::addConnection(std::string &identifier, udp::endpoint *endpoint) {
    std::cout << "[INFO] Recieved handshake from new client " << identifier << std::endl;
    auto conn = new ClientConnection(endpoint);
    connections.insert(std::pair<std::string, ClientConnection*>(identifier, conn));
}

void Server::createPlayer(ClientConnection *connection) {

    auto player = new ServerPlayer(connection, glm::vec3(0, 64, 0));
    players.insert(std::pair<std::string, ServerPlayer*>("USERNAME", player));

    Packet p;
    p = Packet(Packet::PLAYRINFO);

    p.addFloat(player->pos.x);
    p.addFloat(player->pos.y);
    p.addFloat(player->pos.z);

    auto data = p.serialize();

    server_socket->send_to(asio::buffer(data, data.size()), *connection->endpoint);
}

void Server::cleanup() {
    alive = false;

    delete server_socket;
}

Server::~Server() {
    cleanup();
}
