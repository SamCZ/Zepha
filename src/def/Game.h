//
// Created by aurailus on 2020-07-24.
//

#pragma once

#include <memory>

class LuaParser;
class BiomeAtlas;
class DefinitionAtlas;

class Game {
public:
    Game();
    ~Game();

    std::unique_ptr<LuaParser> lua;
    std::unique_ptr<BiomeAtlas> biomes;
    std::unique_ptr<DefinitionAtlas> defs;
};