//
// Created by aurailus on 09/04/19.
//

#ifndef ZEUS_INPUTMANAGER_H
#define ZEUS_INPUTMANAGER_H


class InputManager {
public:
    InputManager();

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
    bool keysDown[1024];
    bool keysPressed[1024];
    bool keysReleased[1024];

    bool leftPressed, leftDown, leftReleased;
    bool rightReleased, rightDown, rightPressed;
};


#endif //ZEUS_INPUTMANAGER_H
