//
// Created by aurailus on 14/10/19.
//

#include "LuaInputManager.h"
#include "../../util/Util.h"
#include <GLFW/glfw3.h>
#include <iostream>

LuaInputManager::LuaInputManager() {
    for (bool& key : keysDown) key = false;
    for (bool& key : keysPressed) key = false;
    for (bool& key : keysReleased) key = false;
    for (auto &callback : callbacksDown) callback = {};
    for (auto &callback : callbacksUp) callback = {};
}

void LuaInputManager::update(bool* keys) {
    for (ushort i = 0; i < 1024; i++) {
        bool key = keys[i];

        if (key) {
            if (!keysDown[i]) keysPressed[i] = true;
            keysDown[i] = true;
        }
        else {
            if (keysDown[i]) keysReleased[i] = true;
            keysDown[i] = false;
        }
    }
}

void LuaInputManager::triggerKeybinds() {
    for (ushort i = 0; i < 1024; i++) {
        if (keysPressed[i]) {
            for (auto& cb : callbacksDown[i]) cb();
            keysPressed[i] = false;
        }
        if (keysReleased[i]) {
            for (auto& cb : callbacksUp[i]) cb();
            keysReleased[i] = false;
        }
    }
}

void LuaInputManager::bindOnDown(ushort key, const sol::function &cb) {
    callbacksDown[key].push_back(cb);
}

void LuaInputManager::bindOnUp(ushort key, const sol::function &cb) {
    callbacksUp[key].push_back(cb);
}
