#pragma once

#include "lua/Lua.h"
#include "util/Types.h"
#include "client/Callback.h"

class Input;
class LocalLuaParser;

namespace Api::Usertype {
	class KeyObserver {
	public:
		KeyObserver(Input& input, LocalLuaParser& parser): input(input), parser(parser), id(ID_NEXT++) {}
		
		~KeyObserver();
		
		void start();
		
		void stop();
		
		static void bind(sol::state& lua, sol::table& core, Input& input, LocalLuaParser& parser);
		
	private:
		sol::protected_function on_press = sol::nil;
		sol::protected_function on_release = sol::nil;
		sol::protected_function on_change = sol::nil;
		
		vec<CallbackRef> nativeCBs {};
		
		usize id = 0;
		bool active = false;
		
		Input& input;
		LocalLuaParser& parser;
		
		static usize ID_NEXT;
	};
	
}