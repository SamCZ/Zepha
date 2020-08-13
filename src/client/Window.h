//
// Created by aurailus on 26/11/18.
//

#pragma once

#include <map>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Input.h"

class Window {
public:
    Window();
    Window(glm::ivec2 win);

    void update();
    bool shouldClose();
    void swapBuffers();

    void addResizeCallback(const std::string& identifier, std::function<void(glm::ivec2)> cb);
    void removeResizeCallback(const std::string& identifier);

    glm::ivec2 getSize();
    void setCursorHand(bool hand);

    ~Window();

    Input input;
    GLFWwindow* mainWindow = nullptr;
private:
    static void scrollCallback(GLFWwindow* window, double xO, double yO);
    static void resizeCallback(GLFWwindow* window, int width, int height);

    GLFWcursor* handCursor = nullptr;

    glm::ivec2 win;
    glm::ivec2 center;

    bool keys[1024] {};

    std::map<std::string, std::function<void(glm::ivec2)>> resizeCallbacks;

};

