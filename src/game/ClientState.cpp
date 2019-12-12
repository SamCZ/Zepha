//
// Created by aurailus on 2019-12-11.
//

#include "ClientState.h"

ClientState::ClientState(const std::string &path, Renderer &renderer) :
    path(path),
    renderer(renderer),
    defs(path + "/assets/textures") {}
