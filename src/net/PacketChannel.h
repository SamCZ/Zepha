//
// Created by aurailus on 08/02/19.
//

#pragma once

enum class PacketChannel {
    UNDEFINED = -1,

    AUTH,
    CONNECT,

    SERVER,

    WORLD,
    ENTITY,
    INTERACT,

    INVENTORY
};

