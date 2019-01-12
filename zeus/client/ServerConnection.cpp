//
// Created by aurailus on 11/01/19.
//

#include "ServerConnection.h"

ServerConnection::ServerConnection(std::string address, int port) {
    this->address = std::move(address);
    this->port = port;
    this->connected = false;
}

ServerConfig* ServerConnection::connect() {
    try {
        asio::ip::udp::resolver resolver(io_context);
        remote_endpoint = *resolver.resolve(asio::ip::udp::v4(), address, std::to_string(port)).begin();

        asio::ip::udp::socket socket(io_context);
        socket.open(asio::ip::udp::v4());

        int attempts = 0;
        bool handshook = false;

        Timer t("Connection time");
        while (t.elapsedNs() < 10L*1000000L*1000L) {

            if (!handshook) {
                Packet p;
                p = Packet(Packet::HANDSHAKE);
                p.addInt(attempts++);
                auto data = p.serialize();

                socket.send_to(asio::buffer(data, data.size()), remote_endpoint);
                std::cout << "Sent handshake." << std::endl;
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(200));

            size_t pendingSize = socket.available();
            if (pendingSize > 0) {
                std::vector<Packet::PacketByte> recv_buf((unsigned long)pendingSize);

                auto remote_endpoint = new asio::ip::udp::endpoint();
                socket.receive_from(asio::buffer(recv_buf, pendingSize), *remote_endpoint);

                auto packet = Packet::deserialize(recv_buf);

                if (packet.type == Packet::HANDSHAKE) {
                    std::cout << "Handshake received." << std::endl;
                    t = Timer("Authenticate time");
                    handshook = true;

                    Packet p;
                    p = Packet(Packet::AUTHTOKEN);
                    p.addString("Hello! I'm a string!!!");
                    auto data = p.serialize();

                    socket.send_to(asio::buffer(data, data.size()), *remote_endpoint);
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

ServerConnection::~ServerConnection() {

}
