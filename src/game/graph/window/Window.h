//
// Created by aurailus on 26/11/18.
//

#pragma once

#include <map>
#include <cstdio>
#include <functional>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>
#include "Input.h"

class Window {
public:
    Window();
    Window(glm::ivec2 win);

    int initialize();

    void update();
    bool shouldClose();
    void swapBuffers();

    void addResizeCallback(const std::string& identifier, std::function<void(glm::ivec2)> cb);
    void removeResizeCallback(const std::string& identifier);

    glm::ivec2 getSize();
    glm::ivec2 getMousePos();
    glm::vec2  getDelta();

    void lockMouse(bool lock);
    void setCursorHand(bool hand);

    ~Window();

    Input input;
    bool keys[1024] {};
private:
    GLFWwindow *mainWindow;

    glm::ivec2 win;
    glm::ivec2 center;
    glm::vec2  delta;

    bool mouseIsLocked = false;
    bool forceMouseUnlocked = false;

    std::map<std::string, std::function<void(glm::ivec2)>> resizeCallbacks;

    //Static so that GLFW can run callback
    static void handleKeys(GLFWwindow* glfwWindow, int key, int code, int action, int mode);
    static void handleResize(GLFWwindow* glfwWindow, int width, int height);

    GLFWcursor* handCursor = nullptr;
};

