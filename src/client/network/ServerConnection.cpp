//
// Created by aurailus on 11/01/19.
//

#include "ServerConnection.h"

ServerConnection::ServerConnection(std::string address, unsigned short port) {
    this->port = port;
    this->address = std::move(address);

    if (enet_initialize() != 0) {
        fprintf(stderr, "[FATAL] Failed to Initialize ENet.\n");
        exit(EXIT_FAILURE);
    }
}

void ServerConnection::init() {
    client = enet_host_create(nullptr, 1, 2, 0, 0);

    if (client == nullptr) {
        fprintf(stderr, "[FATAL] Failed to create ENet client.\n");
        return;
    }

    ENetAddress server_address;

    enet_address_set_host(&server_address, address.c_str());
    server_address.port = port;

    server = enet_host_connect(client, &server_address, 2, 0);

    if (server == nullptr) {
        fprintf(stderr, "No available peers to initiate a connection to.\n");
        return;
    }

    ENetEvent event;
    if (enet_host_service(client, &event, 5000) > 0 && event.type == ENET_EVENT_TYPE_CONNECT) {
        //Connected to the server successfully
        printf("I connected to %x:%u.\n",
               event.peer->address.host,
               event.peer->address.port);
        connected = true;
    }
    else {
        //Connection failed
        enet_peer_reset(server);
        printf("Connection failed.\n");
        connected = false;
    }
}

void ServerConnection::update() {
    ENetEvent event;
    while (enet_host_service(client, &event, 0) > 0) {

        switch (event.type) {
            case ENET_EVENT_TYPE_CONNECT:
                printf("A remote peer connected from %x:%u.\n",
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

    sendInterval++;
//    if (sendInterval % 1 == 0) {

//        std::string pacman("Packet #" + std::to_string(sendCount));
//        sendCount++;
//
//        std::cout << pacman << std::endl;
//
//        ENetPacket* packet = enet_packet_create(pacman.c_str(), pacman.length() + 1, ENET_PACKET_FLAG_RELIABLE);
//        enet_peer_send(server, 0, packet);
//    }
}

void ServerConnection::cleanup() {
    connected = false;
    if (client != nullptr) enet_host_destroy(client);
    enet_deinitialize();
}

ServerConnection::~ServerConnection() {
    cleanup();
}