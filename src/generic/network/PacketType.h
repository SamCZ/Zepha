//
// Created by aurailus on 08/02/19.
//

#ifndef ZEUS_PACKETTYPE_H
#define ZEUS_PACKETTYPE_H


struct PacketType {
    //Type Definitions
    typedef int p_type;

    //Packet Types
    const static p_type UNDEFINED    = 0;
    const static p_type HANDSHAKE    = 1;
    const static p_type AUTHENTICATE = 2;
    const static p_type PLAYERINFO   = 3;
    const static p_type ENTITYINFO   = 4;
    const static p_type CHUNKINFO    = 5;
};


#endif //ZEUS_PACKETTYPE_H
