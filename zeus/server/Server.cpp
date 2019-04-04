//
// Created by aurailus on 09/01/19.
//

#include "Server.h"

Server::Server() = default;

Server::Server(int port) {
    this->port = port;
    alive = true;
}

void Server::start() {
    server_socket = new asio::ip::udp::socket(io_context, udp::endpoint(udp::v4(), 12346));

    auto thread = new std::thread(discovery, &alive, &connections, &connectionsLock);
    thread->detach();

    while (alive) loop();
}

void Server::loop() {
    std::cout << "I am a server" << std::endl;

    long sleep_for = 1L*1000L*1000000L;
    std::this_thread::sleep_for(std::chrono::nanoseconds(sleep_for));
}

void Server::cleanup() {
    alive = false;

    delete server_socket;
}

Server::~Server() {
    cleanup();
}

void Server::discovery(bool* alive, std::vector<ClientConnection *>* connections, std::mutex* lock) {
    std::cout << "Discovery thread active" << std::endl;
//    try {
//
//        while (alive) {
//            char recv_buf[128];
//            udp::endpoint remote_endpoint; //Populated by the next line
//
////            Timer t("owo");
//            socket.receive_from(asio::buffer(recv_buf), remote_endpoint);
////            t.printElapsedSeconds();
//            std::cout << recv_buf << std::endl;
//
//            std::string message = "hey there";
//
//            asio::error_code ignored_error;
//            socket.send_to(asio::buffer(message), remote_endpoint, 0, ignored_error);
//
//            //Socket is garbage collected here, so like, don't do that?
//        }
//    }
//    catch (std::exception& e) {
//        std::cerr << e.what() << std::endl;
//    }
}
