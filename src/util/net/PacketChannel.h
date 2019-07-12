//
// Created by aurailus on 08/02/19.
//

#ifndef ZEUS_PACKETCHANNEL_H
#define ZEUS_PACKETCHANNEL_H

enum class PacketChannel {
    UNDEFINED = -1,
    KEEP_ALIVE,
    AUTHENTICATE,
    CONNECT_DATA,
    PLAYER_INFO,
    ENTITY_INFO,
    CHUNKS,
    SERVER_INFO,
    BLOCK_UPDATES,
};


#endif //ZEUS_PACKETCHANNEL_H
