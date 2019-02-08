//
// Created by aurailus on 08/02/19.
//

#ifndef ZEUS_PACKETCHANNEL_H
#define ZEUS_PACKETCHANNEL_H


struct PacketChannel {
    //Type Definitions
    typedef int p_channel;

    const static int CHANNELS = 12;

    //Packet Channels
    const static p_channel UNDEFINED    = 0;
    const static p_channel KEEPALIVE    = 1;
    const static p_channel AUTHENTICATE = 2;
    const static p_channel PLAYERINFO   = 3;
    const static p_channel ENTITYINFO   = 4;
    const static p_channel WORLDINFO    = 5;

};


#endif //ZEUS_PACKETCHANNEL_H
