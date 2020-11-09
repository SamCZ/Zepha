//
// Created by aurailus on 26/11/18.
//

#include "Window.h"

#include "util/Log.h"

Window::Window() : Window({ 800, 600 }) {};

Window::Window(glm::ivec2 win) :
	win(win), center(win.x / 2, win.y / 2) {
	
	if (!glfwInit()) {
		glfwTerminate();
		throw std::runtime_error("Failed to initialize GLFW context.");
	}
	
	// Version 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	
	// Compatibility - No Backwards compat, only forwards
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	
	// Create the window
	if (!(mainWindow = glfwCreateWindow(win.x, win.y, "Zepha", nullptr, nullptr))) {
		glfwTerminate();
		throw std::runtime_error("Failed to initialize GLFW window.");
	}
	
	glfwGetFramebufferSize(mainWindow, &win.x, &win.y);
	glfwMakeContextCurrent(mainWindow);
	glewExperimental = GL_TRUE;
	
	handCursor = glfwCreateStandardCursor(GLFW_HAND_CURSOR);
	
	// Initialize GLEW
	GLenum error;
	if ((error = glewInit()) != GLEW_OK) {
		glfwTerminate();
		glfwDestroyWindow(mainWindow);
		printf("%s", reinterpret_cast<const char*>(glewGetErrorString(error)));
		throw std::runtime_error("GLEW Fatal error.");
	}
	
	glEnable(GL_DEPTH_TEST);
	glViewport(0, 0, win.x, win.y);
	
	// Setup callbacks
	glfwSetWindowUserPointer(mainWindow, this);
	glfwSetScrollCallback(mainWindow, scrollCallback);
	glfwSetWindowSizeCallback(mainWindow, resizeCallback);
	
	glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	glfwMaximizeWindow(mainWindow);
	
	input.init(mainWindow, &this->win);
}

void Window::update() {
	input.update();
}

bool Window::shouldClose() {
	return static_cast<bool>(glfwWindowShouldClose(mainWindow));
}

void Window::swapBuffers() {
	glfwSwapBuffers(mainWindow);
}

void Window::addResizeCallback(const std::string& identifier, std::function<void(glm::ivec2)> cb) {
	resizeCallbacks.emplace(identifier, cb);
}

void Window::removeResizeCallback(const std::string& identifier) {
	resizeCallbacks.erase(identifier);
}

glm::ivec2 Window::getSize() {
	return win;
}

void Window::setCursorHand(bool hand) {
	glfwSetCursor(mainWindow, hand ? handCursor : nullptr);
}

void Window::scrollCallback(GLFWwindow* window, double xO, double yO) {
//    auto w = static_cast<Window*>(glfwGetWindowUserPointer(window));

}

void Window::resizeCallback(GLFWwindow* window, int width, int height) {
	auto w = static_cast<Window*>(glfwGetWindowUserPointer(window));
	
	glfwGetFramebufferSize(window, &w->win.x, &w->win.y);
	glViewport(0, 0, w->win.x, w->win.y);
	
	w->center = glm::ivec2(w->win.x / 2, w->win.y / 2);
	for (auto& cb : w->resizeCallbacks) cb.second(w->win);
}

Window::~Window() {
	glfwDestroyWindow(mainWindow);
	glfwTerminate();
}