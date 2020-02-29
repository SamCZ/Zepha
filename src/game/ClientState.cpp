//
// Created by aurailus on 2019-12-11.
//

#include "ClientState.h"

#ifdef _WIN32
ClientState::ClientState(const std::string& path, Renderer& renderer) :
    path(path),
    renderer(renderer),
    defs(path + "assets\\textures") {}
#else
ClientState::ClientState(const std::string &path, Renderer &renderer) :
    path(path),
    renderer(renderer),
    defs(path + "/assets/textures") {}
#endif
