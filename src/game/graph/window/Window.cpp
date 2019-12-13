//
// Created by aurailus on 26/11/18.
//

#include <iostream>
#include "Window.h"

Window::Window() : Window({800, 600}) {};

Window::Window(glm::ivec2 win) :
    win(win),
    center(win.x / 2, win.y / 2) {}

int Window::initialize() {
    //Initialize GLFW
    if (!glfwInit()) {
        printf("GLFW init failed\n");
        glfwTerminate();
        return 1;
    }

    //Version 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    //Compatibility - No Backwards compat, only forwards
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    //Create the window
    mainWindow = glfwCreateWindow(win.x, win.y, "Zepha", nullptr, nullptr);
    handCursor = glfwCreateStandardCursor(GLFW_HAND_CURSOR);

    if (!mainWindow) {
        printf("GLFW window failed\n");
        glfwTerminate();
        return 1;
    }

    glfwGetFramebufferSize(mainWindow, &win.x, &win.y);

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
    glViewport(0, 0, win.x, win.y);

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
        delta = {};
    }
    else {
        glfwGetCursorPos(mainWindow, &mouseX, &mouseY);

        if (!mouseIsLocked) {
            if (!forceMouseUnlocked && input.isMousePressed(GLFW_MOUSE_BUTTON_LEFT)) {
                glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
                glfwSetCursorPos(mainWindow, center.x, center.y);
                mouseIsLocked = true;
            }
        }
        else {
            delta = {mouseX - center.x, center.y - mouseY};
            glfwSetCursorPos(mainWindow, center.x, center.y);
        }
    }

    input.update(keys);
    input.updateLeftMouse(glfwGetMouseButton(mainWindow, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS);
    input.updateRightMouse(glfwGetMouseButton(mainWindow, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS);
}

bool Window::shouldClose() {
    return static_cast<bool>(glfwWindowShouldClose(mainWindow));
}

void Window::swapBuffers() {
    glfwSwapBuffers(mainWindow);
}

void Window::addResizeCallback(const std::string &identifier, std::function<void(glm::ivec2)> cb) {
    resizeCallbacks.emplace(identifier, cb);
}

void Window::removeResizeCallback(const std::string &identifier) {
    resizeCallbacks.erase(identifier);
}

glm::ivec2 Window::getSize() {
    return win;
}

glm::ivec2 Window::getMousePos() {
    double xPos, yPos;
    glfwGetCursorPos(mainWindow, &xPos, &yPos);
    return glm::ivec2 {static_cast<int>(xPos), static_cast<int>(yPos)};
}

glm::vec2 Window::getDelta() {
    return delta;
}

void Window::lockMouse(bool lock) {
    forceMouseUnlocked = !lock;
    mouseIsLocked = lock;
    glfwSetCursorPos(mainWindow, center.x, center.y);
    glfwSetInputMode(mainWindow, GLFW_CURSOR, (mouseIsLocked ? GLFW_CURSOR_HIDDEN : GLFW_CURSOR_NORMAL));
    delta = {};
}

void Window::setCursorHand(bool hand) {
    glfwSetCursor(mainWindow, hand ? handCursor : nullptr);
}

Window::~Window() {
    glfwDestroyWindow(mainWindow);
    glfwTerminate();
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

    glfwGetFramebufferSize(glfwWindow, &window->win.x, &window->win.y);
    glViewport(0, 0, window->win.x, window->win.y);

    window->center.x = window->win.x / 2;
    window->center.y = window->win.y / 2;

    for (auto& cb : window->resizeCallbacks) cb.second(window->win);
}
