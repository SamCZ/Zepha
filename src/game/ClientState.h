//
// Created by aurailus on 06/01/19.
//

#pragma once

#include "../def/ClientGame.h"
#include "../net/client/ServerConnection.h"

class Renderer;

class ClientState {
public:
    ClientState(const std::string& path, Renderer& renderer);

    std::string path;
    std::string subgame;

    Renderer& renderer;
    ServerConnection connection {};
    ClientGame defs;

    unsigned int seed = 0;

    std::string desiredState = "this";

    double fps = 0;
    double delta = 0;
};