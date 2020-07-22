//
// Created by aurailus on 07/07/19.
//

#pragma once

enum class PacketType {
    UNDEFINED = 0,

    // Information Request Types
    BLOCK_IDENTIFIER_LIST,
    BIOME_IDENTIFIER_LIST,
    MOD_ORDER,
    MODS,
    MEDIA,
    MEDIA_DONE,
    CONNECT_DATA_RECVD,

    // Miscellaneous
    SERVER_INFO,

    // Player Info
    PLAYER_INFO,
    THIS_PLAYER_INFO,

    // World
    CHUNK,
    MAPBLOCK,

    // Block
    BLOCK_SET,
    BLOCK_INTERACT,

    // Entity
    ENTITY_INFO,
    ENTITY_REMOVED,

    // Inventory
    INV_WATCH,
    INV_UNWATCH,
    INV_INVALID,
    INV_DATA,
    INV_INTERACT
};

