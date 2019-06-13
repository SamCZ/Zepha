//
// Created by aurailus on 12/06/19.
//

#ifndef ZEUS_CALLBACK_H
#define ZEUS_CALLBACK_H

enum Callback {
    CONSTRUCT,
    DESTRUCT,
    AFTER_CONSTRUCT,
    AFTER_DESTRUCT,
    PLACE,
    PLACE_CLIENT,
    BREAK,
    BREAK_CLIENT,
    INTERACT,
    INTERACT_CLIENT,
    HIT,
    HIT_CLIENT
};

#endif //ZEUS_CALLBACK_H
