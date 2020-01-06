//
// Created by aurailus on 07/07/19.
//

#pragma once

enum class PacketType {
    UNDEFINED = 0,
    CONNECT,
    AUTH,

    // Information Request Types
    BLOCK_IDENTIFIER_LIST,
    BIOME_IDENTIFIER_LIST,
    MODS,
    MOD_ORDER,
    MEDIA,
    MEDIA_DONE,
    CONNECT_DATA_RECVD,

    // Ingame Data Types
    SERVER_INFO,
    THIS_PLAYER_INFO,
    PLAYER_INFO,
    MAPBLOCK,
    CHUNK,
    BLOCK_SET,
    ENTITY_INFO
};

