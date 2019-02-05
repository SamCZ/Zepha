//
// Created by aurailus on 09/01/19.
//

#include "Server.h"
#include "../generic/blocks/BlockChunk.h"

Server::Server() = default;

Server::Server(unsigned short port) {
    this->port = port;

    address.host = ENET_HOST_ANY;
    address.port = port;

    if (enet_initialize() != 0) {
        fprintf(stderr, "[FATAL] Failed to Initialize ENet.\n");
        exit(EXIT_FAILURE);
    }
}

void Server::init() {
    server = enet_host_create(&address, 32, 2, 0, 0);

    if (server == nullptr) {
        fprintf(stderr, "[FATAL] Failed to create ENet host.\n");
        exit(EXIT_FAILURE);
    }

    while (alive) update();
}

void Server::update() {
    Timer loop("");

    ENetEvent event;
    while (enet_host_service(server, &event, 0) > 0 && loop.elapsedNs() < 15L*1000000L) {
        switch (event.type) {
            case ENET_EVENT_TYPE_CONNECT:
                printf("A new client connected from %x:%u.\n",
                        event.peer->address.host,
                        event.peer->address.port);

                event.peer->data = (void*)std::string("Fuckoff").c_str();
                break;

            case ENET_EVENT_TYPE_RECEIVE:
                printf("A packet of length %u containing \"%s\" was received from %s on channel %u.\n",
                       (unsigned int)event.packet->dataLength,
                       event.packet->data,
                       (char*)event.peer->data,
                       event.channelID);

                enet_packet_destroy(event.packet);
                break;

            case ENET_EVENT_TYPE_DISCONNECT:
                printf("%s disconnected.\n", (char*)event.peer->data);
                event.peer->data = nullptr;
                break;

            default:
                break;
        }
    }

    long sleep_for = 16L*1000000L - loop.elapsedNs();
    std::this_thread::sleep_for(std::chrono::nanoseconds(sleep_for));
}

void Server::cleanup() {
    alive = false;
    if (server != nullptr) enet_host_destroy(server);
    enet_deinitialize();
}

Server::~Server() {
    cleanup();
}