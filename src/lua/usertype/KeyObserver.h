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
		
		const string& getBuffer();
		void setBuffer(const string& buffer);
		
		static void bind(sol::state& lua, sol::table& core, Input& input, LocalLuaParser& parser);
		
	private:
		usize id = 0;
		bool active = false;
		
		string buffer {};
		
		sol::protected_function on_press = sol::nil;
		sol::protected_function on_release = sol::nil;
		sol::protected_function on_change = sol::nil;
		
		Input& input;
		LocalLuaParser& parser;
		vec<CallbackRef> nativeCBs {};
		
		static usize ID_NEXT;
	};
	
}