//
// Created by aurailus on 26/11/18.
//

#ifndef GLPROJECT_WINDOW_H
#define GLPROJECT_WINDOW_H

#include <stdio.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Window {
public:
    Window();
    Window(GLint windowWidth, GLint windowHeight);

    int initialize();

    GLfloat getBufferWidth() { return bufferWidth; }
    GLfloat getBufferHeight() { return bufferHeight; }

    bool getShouldClose() { return (bool)glfwWindowShouldClose(mainWindow); }

    void swapBuffers() { glfwSwapBuffers(mainWindow); }

    ~Window();

private:
    GLFWwindow *mainWindow;

    GLint width, height;
    GLint bufferWidth, bufferHeight;
};


#endif //GLPROJECT_WINDOW_H
