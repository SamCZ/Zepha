//
// Created by aurailus on 26/11/18.
//

#ifndef GLPROJECT_WINDOW_H
#define GLPROJECT_WINDOW_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Window {
public:
    Window();
    Window(GLint windowWidth, GLint windowHeight);

    int initialize();

    void update();

    GLfloat getBufferWidth() { return bufferWidth; }
    GLfloat getBufferHeight() { return bufferHeight; }

    bool getShouldClose() { return (bool)glfwWindowShouldClose(mainWindow); }

    bool* getKeysArray() { return keys; }

    double getDeltaX();
    double getDeltaY();

    void swapBuffers() { glfwSwapBuffers(mainWindow); }

    ~Window();

private:
    GLFWwindow *mainWindow;

    GLint width, height;
    GLint centerX, centerY;
    GLint bufferWidth, bufferHeight;

    bool keys[1024];

    //Static so that GLFW can run callback
    static void handleKeys(GLFWwindow* glfwWindow, int key, int code, int action, int mode);

    double deltaX;
    double deltaY;
};


#endif //GLPROJECT_WINDOW_H
