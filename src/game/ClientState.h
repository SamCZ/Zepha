//
// Created by aurailus on 06/01/19.
//

#pragma once

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
