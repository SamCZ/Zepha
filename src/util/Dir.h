//
// Created by aurailus on 10/06/19.
//

#pragma once

enum Dir {
    LEFT = 0, RIGHT = 1, TOP = 2, BOTTOM = 3, FRONT = 4, BACK = 5,
    XNEG = 0, XPOS = 1, YPOS = 2, YNEG = 3, ZPOS = 4, ZNEG = 5,

    INVALID = -1, NONE = -1,
    NO_CULL = 6,
};
