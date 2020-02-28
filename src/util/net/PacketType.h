//
// Created by aurailus on 07/07/19.
//

#pragma once

enum class PacketType {
    UNDEFINED = 0,

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
    CHUNK,
    BLOCK_SET,
    ENTITY_INFO,
    ENTITY_REMOVED,

    // Inventory
    WATCH_INV,
    UNWATCH_INV,
    INV_INVALID,
    INVENTORY
};

