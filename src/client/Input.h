//
// Created by aurailus on 09/04/19.
//

#pragma once

#include <array>
#include <functional>
#include <glm/vec2.hpp>

class Input {
public:
    Input();
    void init(GLFWwindow* window, glm::ivec2* winDimensions);
    void setCallback(std::function<void(bool, int)> callback);

    void update();

    bool keyDown(int key);
    bool keyPressed(int key);
    bool keyReleased(int key);

    bool mouseDown(int button);
    bool mousePressed(int button);
    bool mouseReleased(int button);

    void lockMouse(bool lock);
    glm::ivec2 mousePos();
    glm::vec2 mouseDelta();
private:
    void updateMouse(int key);
    static void keyCallback(GLFWwindow* window, int key, int code, int action, int mode);
    static void scrollCallback(GLFWwindow* window, double, double yO);

    GLFWwindow* window = nullptr;
    glm::ivec2* winDimensions = nullptr;

    bool keysNew[1024] {false};
    bool keysDown[1024] {false};
    bool keysPressed[1024] {false};
    bool keysReleased[1024] {false};

    struct mouse { bool down = false; bool pressed = false; bool released = false; };
    std::array<mouse, 3> mouseState {};
    glm::vec2 delta;

    bool mouseIsLocked = false;
    bool forceMouseUnlocked = false;

    std::function<void(bool, int)> callback = nullptr;
};