//
// Created by aurailus on 11/07/19.
//

#ifndef ZEUS_NETSTATE_H
#define ZEUS_NETSTATE_H


enum class NetState {
    UNINITIALIZED,
    FAILED_CONNECT,
    ERROR,
    CLIENT,
    HOST,
    CLOSED
};


#endif //ZEUS_NETSTATE_H
