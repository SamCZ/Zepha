//
// Created by aurailus on 21/07/19.
//

#include <thread>
#include "ServerConnection.h"
#include "../../../util/Log.h"
#include "../../../util/net/NetHandler.h"

void ServerConnection::attemptConnect(Address addr) {
    if (state != State::UNCONNECTED) {
        disconnect();
    }

    if (enet_initialize() != 0) {
        std::cout << Log::err << "Failed to Initialize ENet." << Log::endl;
        state = State::ENET_ERROR;
        return;
    }

    host = enet_host_create(nullptr, 1, NetHandler::PACKET_CHANNELS, 0, 0);

    if (host == nullptr) {
        std::cout << Log::err << "Failed to create ENet client." << Log::endl;
        state = State::ENET_ERROR;
        return;
    }

    ENetAddress eNetAddr;
    enet_address_set_host(&eNetAddr, addr.host.c_str());
    eNetAddr.port = addr.port;

    peer = enet_host_connect(host, &eNetAddr, NetHandler::PACKET_CHANNELS, 0);

    if (peer == nullptr) {
        std::cout << Log::err << "Failed to initialize ENet peer." << Log::endl;
        state = State::ENET_ERROR;
        return;
    }

    connectionTime = std::chrono::high_resolution_clock::now();
    state = State::ATTEMPTING_CONNECT;
}

void ServerConnection::processConnecting() {
    if (state == State::ATTEMPTING_CONNECT) {
        ENetEvent event;

        auto time = std::chrono::high_resolution_clock::now();
        long elapsedMs = std::chrono::duration_cast<std::chrono::milliseconds>(time - connectionTime).count();

        if (elapsedMs < timeout) {
            if (enet_host_service(host, &event, 0) > 0 && event.type == ENET_EVENT_TYPE_CONNECT) {
                std::cout << Log::info << "Connected to "
                          << event.peer->address.host << ":" << event.peer->address.port << "." << Log::endl;

                state = State::CONNECTED;
            }
        }
        else {
            enet_peer_reset(peer);
            if (attempt < attempts) {
                std::cout << Log::info << "Failed to connect to server, retrying." << Log::endl;
                connectionTime = std::chrono::high_resolution_clock::now();
                attempt ++;
            }
            else {
                std::cout << Log::err << "Failed to connect to server." << Log::endl;
                state = State::FAILED_CONNECT;
            }
        }
    }
}

ServerConnection::State ServerConnection::getConnectionStatus() {
    return state;
}

void ServerConnection::disconnect() {
    std::cout << Log::info << "Disconnecting from server." << Log::endl;
    enet_peer_disconnect(peer, 0);
    enet_host_flush(host);
    //TODO: Make sure the client is *actually* disconnected.. get clarification from the docs
    state = State::DISCONNECTED;
}

bool ServerConnection::pollEvents(ENetEvent *event) {
    return enet_host_service(host, event, 0) > 0;
}

ENetPeer *ServerConnection::getPeer() {
    return peer;
}

ServerConnection::~ServerConnection() {
    disconnect();
    delete host;
    delete peer;
}
