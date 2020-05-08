//
// Created by aurailus on 14/10/19.
//

#include "LuaKeybindHandler.h"

#include "parser/LocalLuaParser.h"

LuaKeybindHandler::LuaKeybindHandler(LocalLuaParser* parser): parser(parser) {
    for (auto &callback : callbacksDown) callback = {};
    for (auto &callback : callbacksUp) callback = {};
}

void LuaKeybindHandler::keybindHandler(bool state, int i) {
    if (state) for (auto& cb : callbacksDown[i]) parser->safe_function(cb);
    else for (auto& cb : callbacksUp[i]) parser->safe_function(cb);
}

void LuaKeybindHandler::bindOnDown(unsigned short key, const sol::function &cb) {
    callbacksDown[key].push_back(cb);
}

void LuaKeybindHandler::bindOnUp(unsigned short key, const sol::function &cb) {
    callbacksUp[key].push_back(cb);
}
