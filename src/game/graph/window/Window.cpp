//
// Created by aurailus on 26/11/18.
//

#include "Window.h"

Window::Window() : Window(800, 600) {};

Window::Window(GLint windowWidth, GLint windowHeight) {
    width = windowWidth;
    height = windowHeight;

    centerX = width / 2;
    centerY = height / 2;

    forceMouseUnlocked = false;
    mouseIsLocked = false;
    resized = false;
}

int Window::initialize() {
    //Initialize GLFW
    if (!glfwInit()) {
        printf("GLFW init failed\n");
        glfwTerminate();
        return 1;
    }

    //Version 3.2
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    //Compatibility - No Backwards compat, only forwards
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    //Create the window
    mainWindow = glfwCreateWindow(width, height, "Zeus", nullptr, nullptr);

    if (!mainWindow) {
        printf("GLFW window failed\n");
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

    glEnable(GL_DEPTH_TEST);

    //Setup viewport (draw) size
    glViewport(0, 0, bufferWidth, bufferHeight);

    glfwSetWindowUserPointer(mainWindow, this);
    glfwSetKeyCallback(mainWindow, handleKeys);
    glfwSetWindowSizeCallback(mainWindow, handleResize);
    glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    glfwMaximizeWindow(mainWindow);

    return 0;
}

void Window::update() {
    double mouseX, mouseY;
    if (glfwGetWindowAttrib(mainWindow, GLFW_FOCUSED) == GL_FALSE) {
        mouseIsLocked = false;
        deltaX = 0;
        deltaY = 0;
    }
    else {
        glfwGetCursorPos(mainWindow, &mouseX, &mouseY);

        if (!mouseIsLocked) {
            if (!forceMouseUnlocked && input.isMousePressed(GLFW_MOUSE_BUTTON_LEFT)) {
                glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
                glfwSetCursorPos(mainWindow, centerX, centerY);
                mouseIsLocked = true;
            }
        }
        else {
            deltaX = mouseX - centerX;
            deltaY = centerY - mouseY;
            glfwSetCursorPos(mainWindow, centerX, centerY);
        }
    }

    input.update(keys);
    input.updateLeftMouse(glfwGetMouseButton(mainWindow, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS);
    input.updateRightMouse(glfwGetMouseButton(mainWindow, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS);
}

double Window::getDeltaX() {
    return deltaX;
}

double Window::getDeltaY() {
    return deltaY;
}

void Window::handleKeys(GLFWwindow* glfwWindow, int key, int, int action, int) {
    auto window = static_cast<Window*>(glfwGetWindowUserPointer(glfwWindow));

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(glfwWindow, GL_TRUE);
    }

    if (key > 0 && key < 1024) {
        if (action == GLFW_PRESS) {
            window->keys[key] = true;
        }
        else if (action == GLFW_RELEASE) {
            window->keys[key] = false;
        }
    }
}

void Window::handleResize(GLFWwindow *glfwWindow, int, int) {
    auto window = static_cast<Window*>(glfwGetWindowUserPointer(glfwWindow));

    glfwGetFramebufferSize(glfwWindow, &window->bufferWidth, &window->bufferHeight);
    glViewport(0, 0, window->bufferWidth, window->bufferHeight);
    window->resized = true;

    window->centerX = window->bufferWidth / 2;
    window->centerY = window->bufferHeight / 2;
}

Window::~Window() {
    glfwDestroyWindow(mainWindow);
    glfwTerminate();
}


glm::vec2 Window::getSize() {
    return {bufferWidth, bufferHeight};
}

bool Window::shouldClose() {
    return static_cast<bool>(glfwWindowShouldClose(mainWindow));
}

void Window::swapBuffers() {
    glfwSwapBuffers(mainWindow);
}

void Window::lockMouse(bool lock) {
    forceMouseUnlocked = !lock;
    mouseIsLocked = lock;
    glfwSetCursorPos(mainWindow, centerX, centerY);
    glfwSetInputMode(mainWindow, GLFW_CURSOR, (mouseIsLocked ? GLFW_CURSOR_HIDDEN : GLFW_CURSOR_NORMAL));
    deltaX = 0;
    deltaY = 0;
}
