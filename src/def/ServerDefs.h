//
// Created by aurailus on 10/06/19.
//

#pragma once


#include "ServerDefinitionAtlas.h"
#include "../lua/server/ServerLuaParser.h"
#include "texture/ServerTextureStorage.h"

class ServerWorld;

class ServerDefs {
public:
    void init(ServerWorld& world, const std::string& path);

    ServerDefinitionAtlas& blocks();
    ServerLuaParser& lua();
    ServerTextureStorage& textures();

    void update(double delta);

    ~ServerDefs() = default;
private:
    double delta = 0;

    ServerTextureStorage textureStorage;
    ServerDefinitionAtlas blockAtlas;
    ServerLuaParser luaApi;
};

