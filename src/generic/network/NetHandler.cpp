//
// Created by aurailus on 03/02/19.
//

#include "NetHandler.h"

NetHandler::NetHandler() {
    address = ENetAddress();
    host = nullptr;
    peer = nullptr;
}


NetHandler::NetHandler(std::string host_address, unsigned short host_port) {
    initClient(std::move(host_address), host_port, 3, 3);
}

NetHandler::NetHandler(std::string host_address, unsigned short host_port, int attempts, int timeout) {
    initClient(std::move(host_address), host_port, attempts, timeout);
}

NetHandler::NetHandler(unsigned short port, short max_clients) {
    initServer(port, max_clients);
}

void NetHandler::initServer(unsigned short port, short max_clients) {
    state = HOST;

    if (enet_initialize() != 0) {
        fprintf(stderr, "[FATAL] Failed to Initialize ENet.\n");
        state = ERROR;
        return;
    }

    address.host = ENET_HOST_ANY;
    address.port = port;

    host = enet_host_create(&address, (size_t)max_clients, CHANNELS, 0, 0);
    peer = nullptr;

    if (host == nullptr) {
        fprintf(stderr, "[FATAL] Failed to create ENet host.\n");
        state = ERROR;
        return;
    }

    printf("[INFO] Started server.\n");
}

void NetHandler::initClient(std::string host_address, unsigned short host_port, int attempts, int timeout) {
    state = FAILED_CONNECT;

    if (enet_initialize() != 0) {
        fprintf(stderr, "[FATAL] Failed to Initialize ENet.\n");
        state = ERROR;
        return;
    }

    host = enet_host_create(nullptr, 1, CHANNELS, 0, 0);

    if (host == nullptr) {
        fprintf(stderr, "[FATAL] Failed to create ENet client.\n");
        state = ERROR;
        return;
    }

    enet_address_set_host(&address, host_address.c_str());
    address.port = host_port;

    int attempt = 0;
    while (attempt++ < attempts) {

        peer = enet_host_connect(host, &address, CHANNELS, 0);

        if (peer == nullptr) {
            fprintf(stderr, "[FATAL] Failed to find ENet peer.\n");
            state = ERROR;
            return;
        }

        ENetEvent event;
        if (enet_host_service(host, &event, (enet_uint32)timeout) > 0 && event.type == ENET_EVENT_TYPE_CONNECT) {
            printf("Connected to %x:%u.\n", event.peer->address.host, event.peer->address.port);
            state = CLIENT;
            break;
        } else {
            enet_peer_reset(peer);
            if (attempt < attempts) {
                printf("[INFO] Failed to connect to peer, retrying.\n");
            }
        }
    }

    if (state == FAILED_CONNECT) {
        fprintf(stderr, "[FATAL] Failed to connect to peer.\n");
        return;
    }
}

bool NetHandler::update(ENetEvent *event) {
    return enet_host_service(host, event, 0) > 0;
}

int NetHandler::getState() {
    return state;
}

ENetPeer *NetHandler::getPeer() {
    return peer;
}

NetHandler::~NetHandler() {
    //TODO: This causes a weird bug The destructor is called on initialization for some reason
//    if (host != nullptr) {
//        enet_host_destroy(host);
//    }
//    if (getState() != UNINITIALIZED) {
//        enet_deinitialize();
//    }
}
