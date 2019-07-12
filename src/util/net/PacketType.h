//
// Created by aurailus on 07/07/19.
//

#ifndef ZEUS_PACKETTYPE_H
#define ZEUS_PACKETTYPE_H


enum class PacketType {
    UNDEFINED = 0,
    HANDSHAKE,
    AUTHENTICATE,
    IDENTIFIER_LIST,
    PLAYER_INFO,
    ENTITY_INFO,
    CHUNK_INFO,
    SERVER_INFO,
    BLOCK_SET,
};


#endif //ZEUS_PACKETTYPE_H
