//
// Created by aurailus on 12/06/19.
//

#pragma once

enum class Callback {
    CONSTRUCT,
    AFTER_CONSTRUCT,

    DESTRUCT,
    AFTER_DESTRUCT,

    PLACE,
    PLACE_CLIENT,

    AFTER_PLACE,
    AFTER_PLACE_CLIENT,

    BREAK,
    BREAK_CLIENT,

    AFTER_BREAK,
    AFTER_BREAK_CLIENT,

    INTERACT,
    INTERACT_CLIENT,

    HIT,
    HIT_CLIENT
};
