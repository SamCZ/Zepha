//
// Created by aurailus on 08/02/19.
//

#ifndef ZEUS_PACKETCHANNEL_H
#define ZEUS_PACKETCHANNEL_H


struct PacketChannel {
    const static int CHANNELS = 12;

    enum {
        UNDEFINED = -1,
        KEEP_ALIVE,
        AUTHENTICATE,
        PLAYER_INFO,
        ENTITY_INFO,
        WORLD_INFO
    };
};


#endif //ZEUS_PACKETCHANNEL_H
