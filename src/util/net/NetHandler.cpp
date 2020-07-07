//
// Created by aurailus on 03/02/19.
//

#include <sstream>
#include <iostream>
#include <stdexcept>

#include "NetHandler.h"

#include "Address.h"
#include "../Log.h"

NetHandler::NetHandler(const Address& hostAddress) : NetHandler(std::move(hostAddress), 3, 3) {}

NetHandler::NetHandler(const Address& hostAddress, int attempts, int timeout) {
    initClient(std::move(hostAddress), attempts, timeout);
    initialized = true;
}

NetHandler::NetHandler(unsigned short port, short max_clients) {
    initServer(port, max_clients);
    if (state == NetState::ENET_ERROR) throw std::runtime_error("Failed to initialize ENet.");
}

void NetHandler::initServer(unsigned short port, short max_clients) {
    state = NetState::HOST;

    if (enet_initialize() != 0) {
        state = NetState::ENET_ERROR;
        throw std::runtime_error("Failed to initialize ENet.");
    }

    address.host = ENET_HOST_ANY;
    address.port = port;

    host = enet_host_create(&address, (size_t)max_clients, PACKET_CHANNELS, 0, 0);
    peer = nullptr;

    if (host == nullptr) {
        state = NetState::ENET_ERROR;
        throw std::runtime_error("Failed to create ENet host at port " + std::to_string(address.port) + ".");
    }

    std::cout << Log::info << "Starting Zepha Server on port " << address.port << "." << Log::endl;
}

void NetHandler::initClient(Address hostAddress, int attempts, int timeout) {
    state = NetState::FAILED_CONNECT;

    if (enet_initialize() != 0) {
        fprintf(stderr, "[FATAL] Failed to Initialize ENet.\n");
        state = NetState::ENET_ERROR;
        return;
    }

    host = enet_host_create(nullptr, 1, PACKET_CHANNELS, 0, 0);

    if (host == nullptr) {
        fprintf(stderr, "[FATAL] Failed to create ENet client.\n");
        state = NetState::ENET_ERROR;
        return;
    }

    enet_address_set_host(&address, hostAddress.host.c_str());
    address.port = hostAddress.port;

    int attempt = 0;
    while (attempt++ < attempts) {

        peer = enet_host_connect(host, &address, PACKET_CHANNELS, 0);

        if (peer == nullptr) {
            fprintf(stderr, "[FATAL] Failed to find ENet peer.\n");
            state = NetState::ENET_ERROR;
            return;
        }

        ENetEvent event;
        if (enet_host_service(host, &event, (enet_uint32)timeout) > 0 && event.type == ENET_EVENT_TYPE_CONNECT) {
            std::cout << Log::info << "Connected to "
                      << NetHandler::intToIPString(event.peer->address.host)
                      << ":" << event.peer->address.port << "." << Log::endl;
            state = NetState::CLIENT;
            break;
        }
        else {
            enet_peer_reset(peer);
            if (attempt < attempts) {
                std::cout << Log::info << "Failed to connect to peer, retrying." << Log::endl;
            }
        }
    }

    if (state == NetState::FAILED_CONNECT) {
        std::cout << Log::err << "Failed to connect to peer." << Log::endl;
        return;
    }
}

bool NetHandler::update(ENetEvent *event) {
    return enet_host_service(host, event, 0) > 0;
}

NetState NetHandler::getState() {
    return state;
}

ENetPeer* NetHandler::getPeer() {
    return peer;
}

void NetHandler::disconnect() {
    if (state == NetState::CLIENT) {
        std::cout << Log::info << "Disconnecting from host." << Log::endl;
        enet_peer_disconnect(peer, 0);
        enet_host_flush(host);
    }
    if (state != NetState::HOST) {
        enet_host_destroy(host);
        state = NetState::CLOSED;
    }
}

NetHandler::~NetHandler() {
    if (initialized) {
        if (host != nullptr) {
            disconnect();
        }
        if (getState() != NetState::UNINITIALIZED) {
            enet_deinitialize();
        }
    }
}

std::string NetHandler::intToIPString(unsigned int ip) {
    std::ostringstream s;
    s << ((ip      ) & 0xFF) << ".";
    s << ((ip >> 8 ) & 0xFF) << ".";
    s << ((ip >> 16) & 0xFF) << ".";
    s << ((ip >> 24) & 0xFF);
    return s.str();
}
