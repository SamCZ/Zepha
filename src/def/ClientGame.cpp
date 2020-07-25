//
// Created by aurailus on 18/04/19.
//

#include "ClientGame.h"

#include "gen/LocalBiomeAtlas.h"
#include "LocalDefinitionAtlas.h"
#include "../lua/parser/LocalLuaParser.h"

ClientGame::ClientGame(const std::string& texPath) :
    texPath(texPath),
    textures(2048),

    lua(std::make_unique<LocalLuaParser>()),
    biomes(std::make_unique<LocalBiomeAtlas>()),
    defs(std::make_unique<LocalDefinitionAtlas>(textures)) {

    textures.loadDirectory(texPath);
}

void ClientGame::init(LocalWorld &world, Player& player, ClientState& state) {
    lua->init(*this, world, player, state);
}

void ClientGame::update(double delta) {
    lua->update(delta);
    textures.update();
}

ClientGame::~ClientGame() {}
