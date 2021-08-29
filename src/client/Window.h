//
// Created by aurailus on 26/11/18.
//

#pragma once

#include <list>
#include "util/GL.h"
#include <GLFW/glfw3.h>

#include "util/Types.h"

#include "Input.h"

class Window {
public:
	using RCBLock = sptr<bool>;
	
	Window();
	
	Window(ivec2 win);
	
	void update();
	
	bool shouldClose();
	
	void swapBuffers();
	
	RCBLock onResize(std::function<void(ivec2)> cb);
	
	ivec2 getSize();
	
	void setCursorHand(bool hand);
	
	~Window();
	
	Input input;
	GLFWwindow* mainWindow = nullptr;

private:
	static void scrollCallback(GLFWwindow* window, f64 xO, f64 yO);
	
	static void resizeCallback(GLFWwindow* window, i32 width, i32 height);
	
	GLFWcursor* handCursor = nullptr;
	
	ivec2 win;
	ivec2 center;
	
	std::list<std::pair<RCBLock, std::function<void(ivec2)>>> resizeCallbacks {};
};

