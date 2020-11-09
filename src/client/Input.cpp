//
// Created by aurailus on 09/04/19.
//

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "Input.h"

#include "Window.h"

Input::Input() {
	for (bool& key : keysDown) key = false;
	for (bool& key : keysPressed) key = false;
	for (bool& key : keysReleased) key = false;
	for (bool& key : keysNew) key = false;
}

void Input::init(GLFWwindow* window, glm::ivec2* winDimensions) {
	this->window = window;
	this->winDimensions = winDimensions;
	glfwSetKeyCallback(window, keyCallback);
	glfwSetScrollCallback(window, scrollCallback);
}

void Input::setCallback(std::function<void(bool, int)> callback) {
	this->callback = callback;
}

void Input::update() {
	for (bool& key : keysPressed) key = false;
	for (bool& key : keysReleased) key = false;
	
	for (int i = 0; i < 1024; i++) {
		bool key = keysNew[i];
		
		if (key) {
			if (!keysDown[i]) {
				keysPressed[i] = true;
				if (callback) callback(true, i);
			}
			keysDown[i] = true;
		}
		else {
			if (keysDown[i]) {
				keysReleased[i] = true;
				if (callback) callback(false, i);
			}
			keysDown[i] = false;
		}
	}
	
	for (auto& s : mouseState) {
		s.pressed = false;
		s.released = false;
	}
	
	for (int i = 0; i < 12; i++) {
		keysNew[i] = false;
	}
	
	updateMouse(0);
	updateMouse(1);
	updateMouse(2);
	
	if (glfwGetWindowAttrib(window, GLFW_FOCUSED) == GL_FALSE) {
		mouseIsLocked = false;
		delta = {};
	}
	else {
		double mouseX, mouseY;
		glfwGetCursorPos(window, &mouseX, &mouseY);
		
		if (!mouseIsLocked) {
			if (!forceMouseUnlocked && mousePressed(GLFW_MOUSE_BUTTON_LEFT)) {
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
				glfwSetCursorPos(window, winDimensions->x / 2, winDimensions->y / 2);
				mouseIsLocked = true;
			}
		}
		else {
			delta = { mouseX - (winDimensions->x / 2), -(mouseY - (winDimensions->y / 2)) };
			glfwSetCursorPos(window, winDimensions->x / 2, winDimensions->y / 2);
		}
	}
}

bool Input::keyDown(int key) {
	return keysDown[key];
}

bool Input::keyPressed(int key) {
	return keysPressed[key];
}

bool Input::keyReleased(int key) {
	return keysReleased[key];
}

bool Input::mouseDown(int button) {
	return mouseState[button].down;
}

bool Input::mousePressed(int button) {
	return mouseState[button].pressed;
}

bool Input::mouseReleased(int button) {
	return mouseState[button].released;
}

void Input::updateMouse(int button) {
	if (glfwGetMouseButton(window, button) == GLFW_PRESS) {
		if (!mouseState[button].down) mouseState[button].pressed = true;
		mouseState[button].down = true;
	}
	else {
		if (mouseState[button].down) mouseState[button].released = true;
		mouseState[button].down = false;
	}
}

void Input::keyCallback(GLFWwindow* window, int key, int code, int action, int mode) {
	auto w = static_cast<Window*>(glfwGetWindowUserPointer(window));
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) glfwSetWindowShouldClose(window, GL_TRUE);
	if (key >= 32 && key < 1024) {
		if (action == GLFW_PRESS) w->input.keysNew[key] = true;
		else if (action == GLFW_RELEASE) w->input.keysNew[key] = false;
	}
}

void Input::scrollCallback(GLFWwindow* window, double xO, double yO) {
	auto w = static_cast<Window*>(glfwGetWindowUserPointer(window));
	
	if (xO > 0) {
		//Right
		w->input.keysNew[11] = true;
	}
	else if (xO < 0) {
		// Left
		w->input.keysNew[10] = true;
	}
	
	if (yO > 0) {
		// Up
		w->input.keysNew[8] = true;
	}
	else if (yO < 0) {
		// Down
		w->input.keysNew[9] = true;
	}
}

void Input::lockMouse(bool lock) {
	forceMouseUnlocked = !lock;
	mouseIsLocked = lock;
	glfwSetCursorPos(window, winDimensions->x / 2, winDimensions->y / 2);
	glfwSetInputMode(window, GLFW_CURSOR, (mouseIsLocked ? GLFW_CURSOR_HIDDEN : GLFW_CURSOR_NORMAL));
	delta = {};
}

glm::ivec2 Input::mousePos() {
	double xPos, yPos;
	glfwGetCursorPos(window, &xPos, &yPos);
	return glm::ivec2{ static_cast<int>(xPos), static_cast<int>(yPos) };
}

glm::vec2 Input::mouseDelta() {
	return delta;
}
