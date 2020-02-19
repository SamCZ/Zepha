//
// Created by aurailus on 11/06/19.
//

#pragma once

#include "../LuaParser.h"
#include "ServerModHandler.h"
#include "../../server/conn/ServerClient.h"

class ServerDefs;
class ServerWorld;
class EnetPeer;

class ServerLuaParser : public LuaParser {
public:
    void init(ServerDefs& defs, ServerWorld& world, std::string rootPath);
    void update(double delta) override;

    void sendModsPacket(ENetPeer* peer) const;

    void playerConnected(std::shared_ptr<ServerClient> client);
    void playerDisconnected(std::shared_ptr<ServerClient> client);
private:
    void loadApi(ServerDefs& defs, ServerWorld& world);
    void registerDefs(ServerDefs &defs);

    sol::protected_function_result errorCallback(lua_State*, sol::protected_function_result errPfr);
    sol::protected_function_result runFileSandboxed(const std::string& file);

    ServerModHandler handler;
    double delta = 0;
};

