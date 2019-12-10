//
// Created by aurailus on 07/07/19.
//

#pragma once

enum class PacketType {
    UNDEFINED = 0,
    CONNECT,
    AUTH,

    //Information Request Types
    BLOCK_IDENTIFIER_LIST,
    BIOME_IDENTIFIER_LIST,
    MODS,
    MOD_ORDER,
    MEDIA,
    MEDIA_DONE,
    CONNECT_DATA_RECVD,

    //Ingame Data Types
    PLAYER,
    CHUNK,
    MAPBLOCK,
    ENTITY_INFO,
    SERVER_INFO,
    BLOCK_SET,
};

