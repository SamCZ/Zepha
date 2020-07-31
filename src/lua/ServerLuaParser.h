//
// Created by aurailus on 11/06/19.
//

#pragma once

#include <enet/enet.h>

#include "LuaParser.h"

#include "ServerModHandler.h"

class ServerSubgame;
class ServerWorld;
class ServerPlayer;

class ServerLuaParser : public LuaParser {
public:
    explicit ServerLuaParser(ServerSubgame& game);
    void init(ServerWorld& world, const std::string& rootPath);

    virtual void update(double delta) override;

    void sendModsPacket(ENetPeer* peer) const;

    void playerConnected(std::shared_ptr<ServerPlayer> client);
    void playerDisconnected(std::shared_ptr<ServerPlayer> client);

private:
    void loadApi(ServerSubgame& defs, ServerWorld& world);
    void registerDefs(ServerSubgame &defs);

    virtual sol::protected_function_result errorCallback(sol::protected_function_result r) const override;
    sol::protected_function_result runFileSandboxed(const std::string& file);

    ServerSubgame& game;

    ServerModHandler handler;
    double delta = 0;
};

