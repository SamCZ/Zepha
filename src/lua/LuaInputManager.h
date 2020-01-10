//
// Created by aurailus on 14/10/19.
//

#pragma once

#include <sol2/sol.hpp>

typedef unsigned short ushort;

class LuaInputManager {
public:
    LuaInputManager();

    void update(bool* keys);
    void triggerKeybinds();

    void bindOnDown(ushort key, const sol::function& cb);
    void bindOnUp(ushort key, const sol::function& cb);
private:
    bool keysDown[1024] {};
    bool keysPressed[1024] {};
    bool keysReleased[1024] {};

    std::array<std::vector<sol::function>, 1024> callbacksDown {};
    std::array<std::vector<sol::function>, 1024> callbacksUp {};
};