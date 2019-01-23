//
// Created by aurailus on 11/01/19.
//

#include "ServerConnection.h"

ServerConnection::ServerConnection(std::string address, int port) : socket(io_context) {
    this->address = std::move(address);
    this->port = port;
    this->connected = false;
}

ServerConfig* ServerConnection::connect() {
    try {
        asio::ip::udp::resolver resolver(io_context);
        remote_endpoint = *resolver.resolve(asio::ip::udp::v4(), address, std::to_string(port)).begin();

        socket.open(asio::ip::udp::v4());

        int attempts = 0;
        bool handshook = false;

        Timer t("Connection time");
        while (t.elapsedNs() < 10L*1000000L*1000L) {

            if (!handshook) {
                Packet p;
                p = Packet(Packet::HANDSHAKE);
                p.addInt(attempts++);

                sendPacket(p, remote_endpoint);
                std::cout << "Sent handshake." << std::endl;
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(200));

            size_t pendingSize = socket.available();
            if (pendingSize > 0) {
                std::vector<Packet::PacketByte> recv_buf((unsigned long)pendingSize);

                auto ignored = new asio::ip::udp::endpoint();
                socket.receive_from(asio::buffer(recv_buf, pendingSize), *ignored);

                auto packet = Packet::deserialize(recv_buf);

                if (packet.type == Packet::HANDSHAKE) {
                    std::cout << "Handshake received." << std::endl;
                    t = Timer("Authenticate time");
                    handshook = true;

                    Packet p;
                    p = Packet(Packet::AUTHTOKEN);
                    p.addString("Hello! I'm a string!!!");

                    sendPacket(p, remote_endpoint);
                }
                else if (packet.type == Packet::PLAYRINFO) {
                    std::cout << "Player info received." << std::endl;
                    connected = true;

                    float x = Packet::decodeFloat(&packet.data[0]);
                    float y = Packet::decodeFloat(&packet.data[4]);
                    float z = Packet::decodeFloat(&packet.data[8]);

                    return new ServerConfig {
                        .playerPos = glm::vec3(x, y, z)
                    };
                }
            }
        }
        if (!connected) {
            std::cout << "Connection timed out!" << std::endl;
            return nullptr;
        }
    }
    catch (std::exception &e) {
        std::cout << e.what() << std::endl;
    }
}

void ServerConnection::update() {
    Timer t("Loop time");

    //Collect incoming packets
    while (socket.available() > 0) {
        size_t pendingSize = socket.available();
        std::vector<Packet::PacketByte> recv_buf((unsigned long)pendingSize);

        auto remote_endpoint = new asio::ip::udp::endpoint();
        socket.receive_from(asio::buffer(recv_buf, pendingSize), *remote_endpoint);

        auto packet = Packet::deserialize(recv_buf);
        if (packet.length > 0) handlePacket(packet, remote_endpoint);
    }

    long sleep_for = 16L*1000000L - t.elapsedNs();
    std::this_thread::sleep_for(std::chrono::nanoseconds(sleep_for));
}

void ServerConnection::handlePacket(Packet &packet, asio::ip::udp::endpoint* endpoint) {

}

void ServerConnection::sendPacket(Packet &p, asio::ip::udp::endpoint &e) {
    auto data = p.serialize();
    socket.send_to(asio::buffer(data, data.size()), e);
}

void ServerConnection::reqChunks(glm::vec3 a, glm::vec3 b) {

    Packet p(Packet::REQCHUNKS);

    for (int i = (int)a.x; i < (int)b.x; i++) {
        for (int j = (int)a.y; j < (int)b.y; j++) {
            for (int k = (int)a.z; k < (int)b.z; k++) {
                p.addInt(i);
                p.addInt(j);
                p.addInt(k);
            }
        }
    }

    sendPacket(p, remote_endpoint);
}

ServerConnection::~ServerConnection() = default;