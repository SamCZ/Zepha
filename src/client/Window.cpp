//
// Created by aurailus on 26/11/18.
//

#include "Window.h"

#include <stdexcept>

#include "util/Log.h"
#include "util/GL.h"

Window::Window() : Window({ 800, 600 }) {};

Window::Window(ivec2 win) :
	win(win) {
	
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
	
	handCursor = glfwCreateStandardCursor(GLFW_HAND_CURSOR);
	
	// Initialize GLEW
	if(!gladLoadGL()) {
		printf("Something went wrong!\n");
		exit(-1);
	}
	
	glEnable(GL_DEPTH_TEST);
	glViewport(0, 0, win.x, win.y);
	
	// Setup callbacks
	glfwSetWindowUserPointer(mainWindow, this);
	glfwSetWindowSizeCallback(mainWindow, resizeCallback);
	
	glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	glfwMaximizeWindow(mainWindow);
	
	input.init(mainWindow);
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

ivec2 Window::getSize() {
	return win;
}

void Window::setCursorHand(bool hand) {
	glfwSetCursor(mainWindow, hand ? handCursor : nullptr);
}

void Window::resizeCallback(GLFWwindow* window, i32 width, i32 height) {
	let w = static_cast<Window*>(glfwGetWindowUserPointer(window));
	
	glfwGetFramebufferSize(window, &w->win.x, &w->win.y);
	glViewport(0, 0, w->win.x, w->win.y);
	w->resize.call(ivec2 { width, height });
}

Window::~Window() {
	glfwDestroyWindow(mainWindow);
	glfwTerminate();
}