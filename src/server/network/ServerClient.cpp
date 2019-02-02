//
// Created by aurailus on 10/01/19.
//

#include "ServerClient.h"

ServerClient::ServerClient() = default;

ServerClient::ServerClient(asio::ip::udp::endpoint *endpoint, std::string uuid) {
    this->endpoint = endpoint;
    this->uuid = std::move(uuid);
    this->authenticated = false;
}

ServerClient::~ServerClient() {
    delete this->endpoint;
}