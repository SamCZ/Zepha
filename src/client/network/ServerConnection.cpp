//
// Created by aurailus on 11/01/19.
//

#include "ServerConnection.h"

ServerConnection::ServerConnection(std::string address, unsigned short port) {
    this->port = port;
    this->address = std::move(address);
}

void ServerConnection::init() {
    handler = NetHandler(address, port, 3, 3000);

    if (handler.getState() != NetHandler::CLIENT) {
        exit(EXIT_FAILURE);
    }
}

void ServerConnection::update() {
    ENetEvent event;
    while (handler.update(&event)) {

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
}

ServerConnection::~ServerConnection() {
    cleanup();
}