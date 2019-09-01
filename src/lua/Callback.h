//
// Created by aurailus on 12/06/19.
//

#pragma once

enum class Callback {
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
