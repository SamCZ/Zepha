//
// Created by aurailus on 26/11/18.
//

#ifndef GLPROJECT_WINDOW_H
#define GLPROJECT_WINDOW_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cstdio>

class Window {
public:
    Window();
    Window(GLint windowWidth, GLint windowHeight);

    int initialize();

    void update();

    GLfloat getBufferWidth();
    GLfloat getBufferHeight();
    bool resized;

    bool getShouldClose();

    bool* getKeysArray();
    bool mouseIsDown();

    double getDeltaX();
    double getDeltaY();

    void swapBuffers();

    ~Window();

private:
    GLFWwindow *mainWindow;

    GLint width, height;
    GLint centerX, centerY;
    bool mouseLocked;

    GLint bufferWidth, bufferHeight;

    bool keys[1024];
    bool mouseDown;

    //Static so that GLFW can run callback
    static void handleKeys(GLFWwindow* glfwWindow, int key, int code, int action, int mode);
    static void handleResize(GLFWwindow* glfwWindow, int width, int height);

    double deltaX;
    double deltaY;
};


#endif //GLPROJECT_WINDOW_H
