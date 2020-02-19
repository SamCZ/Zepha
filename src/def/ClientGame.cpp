//
// Created by aurailus on 18/04/19.
//

#include "ClientGame.h"

//Note: This constructor is only for *uninitialized* LocalDef objects!
ClientGame::ClientGame(const ClientGame &copy) : ClientGame(copy.tex_path) {}

ClientGame::ClientGame(const std::string& path) :
        tex_path(path),

        parser(),
        biomes(),
        textures(2048),
        defs(textures) {

    textures.loadDirectory(tex_path);
}

void ClientGame::init(LocalWorld &world, Player& player) {
    parser.init(*this, world, player);
}

void ClientGame::update(double delta, bool* keys) {
    parser.update(delta, keys);
}
