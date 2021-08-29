#include "util/GL.h"
#include <util/Util.h>
#include <GLFW/glfw3.h>

#include "Input.h"

#include "Window.h"

void Input::init(GLFWwindow* window) {
	this->window = window;
	glfwSetKeyCallback(window, keyCallback);
	glfwSetScrollCallback(window, scrollCallback);
	glfwSetMouseButtonCallback(window, mouseCallback);
}

void Input::update() {
	mouseDelta = {};
	if (glfwGetWindowAttrib(window, GLFW_FOCUSED) == GL_FALSE) mouseLocked = false;
	else {
		f64 mouseX, mouseY;
		glfwGetCursorPos(window, &mouseX, &mouseY);
		
		if (!mouseLocked) {
			if (!forceMouseUnlocked && isMouseDown(GLFW_MOUSE_BUTTON_LEFT)) {
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
				glfwSetCursorPos(window, LOCKED_MOUSE_POS.x, LOCKED_MOUSE_POS.y);
				mouseLocked = true;
			}
		}
		else {
			mouseDelta = { mouseX - LOCKED_MOUSE_POS.x, -mouseY + LOCKED_MOUSE_POS.y };
			glfwSetCursorPos(window, LOCKED_MOUSE_POS.x, LOCKED_MOUSE_POS.y);
		}
	}
}

bool Input::isKeyDown(u32 key) {
	return keyState[key];
}

bool Input::isMouseDown(u32 button) {
	return mouseState[button];
}

void Input::setMouseLocked(bool lock) {
	forceMouseUnlocked = !lock;
	mouseLocked = lock;
	glfwSetCursorPos(window, LOCKED_MOUSE_POS.x, LOCKED_MOUSE_POS.y);
	glfwSetInputMode(window, GLFW_CURSOR, (mouseLocked ? GLFW_CURSOR_HIDDEN : GLFW_CURSOR_NORMAL));
}

usize Input::bindKeyCallback(i32 key, std::function<void(i32)> cb) {
	keyCallbacks[key].emplace(keyCallbacksInd, ICB(cb));
	return keyCallbacksInd++;
}

void Input::unbindKeyCallback(i32 key, usize id) {
	keyCallbacks[key].at(id).valid = false;
}

usize Input::bindKeyCallback(std::function<void(u32, i32)> cb) {
	globalKeyCallbacks.emplace(globalKeyCallbacksInd, ICB(cb));
	return globalKeyCallbacksInd++;
}

void Input::unbindKeyCallback(usize id) {
	globalKeyCallbacks.at(id).valid = false;
}

usize Input::bindMouseCallback(i32 button, std::function<void(i32)> cb) {
	mouseCallbacks[button].emplace(mouseCallbacksInd, ICB(cb));
	return mouseCallbacksInd++;
}

void Input::unbindMouseCallback(i32 button, usize id) {
	mouseCallbacks[button].at(id).valid = false;
}

usize Input::bindMouseCallback(std::function<void(u32, i32)> cb) {
	globalMouseCallbacks.emplace(globalMouseCallbacksInd, ICB(cb));
	return globalMouseCallbacksInd++;
}

void Input::unbindMouseCallback(usize id) {
	globalMouseCallbacks.at(id).valid = false;
}

ivec2 Input::getMousePos() {
	f64 xPos, yPos;
	glfwGetCursorPos(window, &xPos, &yPos);
	return ivec2 { xPos, yPos };
}

ivec2 Input::getMouseDelta() {
	return mouseDelta;
}

void Input::updateKey(u32 key, i32 state) {
	keyState[key] = state != GLFW_RELEASE && state != 3;
	for (let& cb : keyCallbacks[key]) if (cb.second.valid) cb.second.cb(state);
	for (let& cb : globalKeyCallbacks) if (cb.second.valid) cb.second.cb(key, state);
}

void Input::updateMouse(u32 button, i32 state) {
	mouseState[button] = state != GLFW_RELEASE && state != 3;
	for (let& cb : mouseCallbacks[button]) if (cb.second.valid) cb.second.cb(state);
	for (let& cb : globalMouseCallbacks) if (cb.second.valid) cb.second.cb(button, state);
}

void Input::scrollCallback(GLFWwindow* window, f64 x, f64 y) {
	let& self = static_cast<Window*>(glfwGetWindowUserPointer(window))->input;
	self.updateKey(x > 0 ? 11 : x < 0 ? 10 : y > 0 ? 8 : 9, 3);
}

void Input::keyCallback(GLFWwindow* window, i32 key, i32, i32 action, i32) {
	let& self = static_cast<Window*>(glfwGetWindowUserPointer(window))->input;
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) glfwSetWindowShouldClose(window, GL_TRUE);
	else if (key >= 32 && key < 1024) self.updateKey(key, action);
}

void Input::mouseCallback(GLFWwindow* window, i32 button, i32 action, i32) {
	let& self = static_cast<Window*>(glfwGetWindowUserPointer(window))->input;
	self.updateMouse(button, action);
}

const ivec2 Input::LOCKED_MOUSE_POS = { 200, 200 };
