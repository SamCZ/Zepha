//
// Created by aurailus on 07/07/19.
//

#ifndef ZEUS_PACKETTYPE_H
#define ZEUS_PACKETTYPE_H

enum class PacketType {
    UNDEFINED = 0,
    CONNECT,
    AUTH,

    //Information Request Types
    IDENTIFIER_LIST,
    MODS,
    MOD_ORDER,
    MEDIA,
    MEDIA_DONE,
    CONNECT_DATA_RECVD,

    //Ingame Data Types
    PLAYER,
    CHUNK,
    ENTITY_INFO,
    SERVER_INFO,
    BLOCK_SET,
};


#endif //ZEUS_PACKETTYPE_H
