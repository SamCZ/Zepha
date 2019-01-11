//
// Created by aurailus on 09/01/19.
//

#include "Server.h"

Server::Server() = default;

Server::Server(int port) {
    //TODO: Use the port in the server initializer
    this->port = port;
    alive = true;
}

void Server::start() {
    server_socket = new asio::ip::udp::socket(io_context, udp::endpoint(udp::v4(), 12346));

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

void Server::handlePacket(Packet &packet, udp::endpoint* endpoint) {
    std::string identifier = endpoint->address().to_string() + ":" + std::to_string(endpoint->port());

    if (packet.type == Packet::HANDSHAKE) {
        if (connections.count(identifier) == 0) {
            std::cout << "Added " << identifier << " to client connections." << std::endl;
            auto conn = new ClientConnection(endpoint);
            connections.insert(std::pair<std::string, ClientConnection*>(identifier, conn));
        }
        else {
            std::cout << identifier << " is already added to client connections." << std::endl;
        }
        return;
    }
    if (connections.count(identifier) == 0) {
        std::cout << "Non-handshake packet from identifier " << identifier << "." << std::endl;
        return;
    }

    std::cout << packet.type << ", " << Packet::decodeInt(&packet.data[0]) << std::endl;
}

void Server::cleanup() {
    alive = false;

    delete server_socket;
}

Server::~Server() {
    cleanup();
}