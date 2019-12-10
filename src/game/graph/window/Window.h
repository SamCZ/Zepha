//
// Created by aurailus on 26/11/18.
//

#pragma once

#include <cstdio>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>
#include "InputManager.h"

class Window {
public:
    Window();
    Window(GLint windowWidth, GLint windowHeight);

    int initialize();

    void update();

    glm::vec2 getSize();
    bool resized;

    bool shouldClose();

    glm::ivec2 getMousePos();
    void lockMouse(bool lock);
    double getDeltaX();
    double getDeltaY();

    void swapBuffers();

    ~Window();

    InputManager input;
    bool keys[1024] {};
private:
    GLFWwindow *mainWindow;

    GLint width, height;
    GLint centerX, centerY;
    bool mouseIsLocked, forceMouseUnlocked;

    GLint bufferWidth, bufferHeight;

    //Static so that GLFW can run callback
    static void handleKeys(GLFWwindow* glfwWindow, int key, int code, int action, int mode);
    static void handleResize(GLFWwindow* glfwWindow, int width, int height);

    double deltaX = 0;
    double deltaY = 0;
};

