#pragma once

#include <list>
#include <functional>
#include <unordered_map>

#include "util/Types.h"
#include "client/Callback.h"

/**
 * Manages callbacks for key and mouse input, allows toggling mouse locking.
 */

class Input {
public:
	
	/** Activates the input listeners. */
	void init(GLFWwindow* window);
	
	/** Updates non-listener based data. */
	void update();
	
	/** Returns true if the specified key is down.  */
	bool isKeyDown(u32 key);
	
	/** Returns true if the specified mouse button is down. */
	bool isMouseDown(u32 button);
	
	/** Sets whether or not the mouse is locked and hidden for first person camera use. */
	void setMouseLocked(bool lock);
	
	/**
	 * Gets the position of the mouse relative to the screen.
	 * Will always report the center of the screen if the mouse is locked.
	 */
	
	ivec2 getMousePos();
	
	/**
	 * Gets the delta of the last mouse movement.
	 * Only works if the mouse is locked.
	 */
	 
	ivec2 getMouseDelta();
	
	enum class CBType {
		KEY,
		KEY_PRESS,
		KEY_RELEASE,
		MOUSE,
		MOUSE_PRESS,
		MOUSE_RELEASE,
		_END
	};
	
	CallbackGroup<CBType, u32, i32> events {};
	
private:
	
	/** Calls the key callbacks and sets the key state of the key provided. */
	void updateKey(u32 key, i32 state);
	
	/** Calls the button callbacks and sets the button state of the mouse button provided. */
	void updateMouse(u32 button, i32 state);
	
	/**
	 * Calls the following key callbacks when the user scrolls.
	 * Right: 11, Left: 10, Up: 8, Down: 9
	 */
	 
	static void scrollCallback(GLFWwindow* window, f64 x, f64 y);
	
	/** Calls the updateKey function with the proper key and state. */
	static void keyCallback(GLFWwindow* window, i32 key, i32 code, i32 action, i32 mode);
	
	/** Calls the updateMouse function with the proper button and state. */
	static void mouseCallback(GLFWwindow* window, i32 key, i32 action, i32 mods);
	
	const static ivec2 LOCKED_MOUSE_POS;
	
	array<bool, 1024> keyState {};
	array<bool, 3> mouseState {};
	
	vec2 mouseDelta;
	bool mouseLocked = false;
	bool forceMouseUnlocked = false;
	
	GLFWwindow* window = nullptr;
};