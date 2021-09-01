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
	Window();
	
	Window(ivec2 win);
	
	void update();
	
	bool shouldClose();
	
	void swapBuffers();
	
	ivec2 getSize();
	
	void setCursorHand(bool hand);
	
	~Window();
	
	Input input;
	
	GLFWwindow* mainWindow = nullptr;
	
	CallbackManager<ivec2> resize {};
	
private:
	static void resizeCallback(GLFWwindow* window, i32 width, i32 height);
	
	GLFWcursor* handCursor = nullptr;
	
	ivec2 win;
};

