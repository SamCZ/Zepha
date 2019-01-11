//
// Created by aurailus on 10/01/19.
//

#include "ClientConnection.h"

ClientConnection::ClientConnection() = default;

ClientConnection::ClientConnection(asio::ip::udp::endpoint* endpoint) {
    this->endpoint = endpoint;
}

ClientConnection::~ClientConnection() {
    delete this->endpoint;
}