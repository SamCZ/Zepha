//
// The ClientGame class stores all of the subgame data for the client.
// This data is used when in the GameScene. It is initialized when the client joins a game and cleared when they exit it.
// The data within is in an undefined state until the init method is called.
// Created by aurailus on 18/04/19.
//

#pragma once

#include "Subgame.h"

#include "model/ModelStore.h"
#include "gen/LocalBiomeAtlas.h"
#include "LocalDefinitionAtlas.h"
#include "texture/TextureAtlas.h"
#include "../lua/LocalLuaParser.h"

class LocalPlayer;
class LocalWorld;
class ClientState;

class LocalSubgame : public Subgame {
public:
    explicit LocalSubgame(const std::string& texPath);
    ~LocalSubgame();

    void initApi(LocalWorld &world, ClientState& state);
    void loadPlayer(std::shared_ptr<LocalPlayer> player);
    void update(double delta);

    std::string texPath;

    LocalDefinitionAtlas& getDefs() override { return *defs; };
    LocalBiomeAtlas& getBiomes() override { return *biomes; };
    LocalLuaParser& getParser() override { return *lua; };

    ModelStore models;
    TextureAtlas textures;

private:
    std::unique_ptr<LocalLuaParser> lua;
    std::unique_ptr<LocalBiomeAtlas> biomes;
    std::unique_ptr<LocalDefinitionAtlas> defs;
};

