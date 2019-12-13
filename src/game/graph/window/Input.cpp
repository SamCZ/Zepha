//
// Created by aurailus on 09/04/19.
//

#include <GLFW/glfw3.h>
#include "Input.h"

Input::Input() {
    for (bool& key : keysDown) key = false;
    for (bool& key : keysPressed) key = false;
    for (bool& key : keysReleased) key = false;

    leftDown = false;
    rightDown = false;
    leftPressed = false;
    rightPressed = false;
    leftReleased = false;
    rightReleased = false;
}

void Input::update(bool* keys) {
    for (bool &key : keysPressed) key = false;
    for (bool &key : keysReleased) key = false;

    for (int i = 0; i < 1024; i++) {
        bool key = keys[i];

        if (key) {
            if (!keysDown[i]) keysPressed[i] = true;
            keysDown[i] = true;
        }
        else {
            if (!keysReleased[i]) keysReleased[i] = true;
            keysDown[i] = false;
        }
    }

    leftPressed = false;
    leftReleased = false;

    rightPressed = false;
    rightReleased = false;
}

void Input::updateLeftMouse(bool down) {
    if (down) {
        if (!leftDown) leftPressed = true;
        leftDown = true;
    }
    else {
        if (leftDown) leftReleased = true;
        leftDown = false;
    }
}

void Input::updateRightMouse(bool down) {
    if (down) {
        if (!rightDown) rightPressed = true;
        rightDown = true;
    }
    else {
        if (rightDown) rightReleased = true;
        rightDown = false;
    }
}

bool Input::isKeyDown(int key) {
    return keysDown[key];
}

bool Input::isKeyPressed(int key) {
    return keysPressed[key];
}

bool Input::isKeyReleased(int key) {
    return keysReleased[key];
}

bool Input::isMouseDown(int button) {
    if      (button == GLFW_MOUSE_BUTTON_LEFT)  return leftDown;
    else if (button == GLFW_MOUSE_BUTTON_RIGHT) return rightDown;
    return false;
}

bool Input::isMousePressed(int button) {
    if      (button == GLFW_MOUSE_BUTTON_LEFT)  return leftPressed;
    else if (button == GLFW_MOUSE_BUTTON_RIGHT) return rightPressed;
    return false;
}

bool Input::isMouseReleased(int button) {
    if      (button == GLFW_MOUSE_BUTTON_LEFT)  return leftReleased;
    else if (button == GLFW_MOUSE_BUTTON_RIGHT) return rightReleased;
    return false;
}
