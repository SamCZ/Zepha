//
// Created by aurailus on 26/11/18.
//

#include "Window.h"

Window::Window() {
    width = 800;
    height = 600;
}

Window::Window(GLint windowWidth, GLint windowHeight) {
    width = windowWidth;
    height = windowHeight;
}

int Window::initialize() {
    //Initialize GLFW
    if (!glfwInit()) {
        printf("GLFW init failed");
        glfwTerminate();
        return 1;
    }

    //Version 3.2
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    //Compatibility - No Backwards compat, only forwards
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    //MSAA
    glfwWindowHint(GLFW_SAMPLES, 16);
    glEnable(GL_MULTISAMPLE);

    //Create the window
    mainWindow = glfwCreateWindow(width, height, "OpenGL Learning", nullptr, nullptr);

    if (!mainWindow) {
        printf("GLFW window failed");
        glfwTerminate();
        return 1;
    }

    glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

    //Set context for GLEW to our window
    glfwMakeContextCurrent(mainWindow);

    //Allow modern extension features
    glewExperimental = GL_TRUE;

    //Initialize GLEW
    GLenum error;
    if ((error = glewInit()) != GLEW_OK) {
        printf("GLEW init failed.");
        fprintf(stderr, "Error: %s\n", glewGetErrorString(error));
        glfwDestroyWindow(mainWindow);
        glfwTerminate();
        return 1;
    }

    //Setup viewport (draw) size
    glViewport(0, 0, bufferWidth, bufferHeight);

    glEnable(GL_DEPTH_TEST);

    return 0;
}

Window::~Window() {
    glfwDestroyWindow(mainWindow);
    glfwTerminate();
}