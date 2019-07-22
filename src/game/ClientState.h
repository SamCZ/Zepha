//
// Created by aurailus on 06/01/19.
//

#ifndef ZEUS_CLIENTSTATE_H
#define ZEUS_CLIENTSTATE_H

#include "graph/Renderer.h"
#include "scene/net/ClientNetworkInterpreter.h"
#include "../def/LocalDefs.h"
#include "scene/net/ServerConnection.h"

struct ClientState {
    Renderer& renderer;
    ServerConnection connection;
    LocalDefs defs;

    //TODO: Not this
    std::string desiredState;

    double fps = 0;
    double deltaTime = 0;
};

#endif //ZEUS_CLIENTSTATE_H
