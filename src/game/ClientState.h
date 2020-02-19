//
// Created by aurailus on 06/01/19.
//

#pragma once

#include "graph/Renderer.h"
#include "scene/net/ClientNetworkInterpreter.h"
#include "../def/ClientGame.h"
#include "scene/net/ServerConnection.h"

class ClientState {
public:
    ClientState(const std::string& path, Renderer& renderer);

    std::string path;
    std::string subgame;

    Renderer& renderer;
    ServerConnection connection {};
    ClientGame defs;

    std::string desiredState = "this";

    double fps = 0;
    double deltaTime = 0;
};