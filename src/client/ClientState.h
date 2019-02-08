//
// Created by aurailus on 06/01/19.
//

#ifndef ZEUS_CLIENTSTATE_H
#define ZEUS_CLIENTSTATE_H

#include "engine/graphics/Renderer.h"

struct ClientState {
    Renderer* renderer;

    double fps;
    double deltaTime;
};

#endif //ZEUS_CLIENTSTATE_H
