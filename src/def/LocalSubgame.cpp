//
// Created by aurailus on 18/04/19.
//

#include "LocalSubgame.h"

LocalSubgame::LocalSubgame(const std::string& texPath) :
    texPath(texPath),
    textures(2048),

    lua(std::make_unique<LocalLuaParser>(*this)),
    biomes(std::make_unique<LocalBiomeAtlas>()),
    defs(std::make_unique<LocalDefinitionAtlas>(textures)) {

    textures.loadDirectory(texPath);
}

void LocalSubgame::initApi(LocalWorld &world, ClientState& state) {
    lua->init(world, state);
}

void LocalSubgame::loadPlayer(std::shared_ptr<LocalPlayer> player) {
    lua->loadPlayer(player);
}

void LocalSubgame::update(double delta) {
    lua->update(delta);
    textures.update();
}

LocalSubgame::~LocalSubgame() {}
