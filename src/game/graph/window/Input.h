//
// Created by aurailus on 09/04/19.
//

#pragma once

class Input {
public:
    Input();

    void update(bool* keys);
    void updateLeftMouse(bool down);
    void updateRightMouse(bool down);

    bool isKeyDown(int key);
    bool isKeyPressed(int key);
    bool isKeyReleased(int key);

    bool isMouseDown(int button);
    bool isMousePressed(int button);
    bool isMouseReleased(int button);
private:
    bool keysDown[1024] {false};
    bool keysPressed[1024] {false};
    bool keysReleased[1024] {false};

    bool leftPressed, leftDown, leftReleased;
    bool rightReleased, rightDown, rightPressed;
};