//
// Created by aurailus on 11/06/19.
//

#pragma once

#include "ServerModHandler.h"
#include "../LuaParser.h"
#include "../../server/conn/ServerClient.h"

class ServerGame;
class ServerWorld;

class ServerLuaParser : public LuaParser {
public:
    void init(ServerGame& defs, ServerWorld& world, std::string rootPath);
    void update(double delta) override;

    void sendModsPacket(ENetPeer* peer) const;

    void playerConnected(std::shared_ptr<ServerClient> client);
    void playerDisconnected(std::shared_ptr<ServerClient> client);
private:
    void loadApi(ServerGame& defs, ServerWorld& world);
    void registerDefs(ServerGame &defs);

    sol::protected_function_result errorCallback(lua_State*, sol::protected_function_result errPfr);
    sol::protected_function_result runFileSandboxed(const std::string& file);

    ServerModHandler handler;
    double delta = 0;
};

