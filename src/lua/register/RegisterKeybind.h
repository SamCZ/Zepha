#pragma once

#include "lua/Lua.h"
#include "game/LocalSubgame.h"
#include "game/ServerSubgame.h"
#include "game/def/BiomeDef.h"

namespace RegisterKeybind {
	namespace {
		
		/**
		 * Registers a keybind from the keybinds table to the handler provided.
		 *
		 * @param keybinds - The keybinds table to index.
		 * @param identifier - The identifier of the keybind in the table.
		 * @param handler - The keybind handler to add the keybind to.
		 */
		
		static void registerKeybind(Input& input, vec<CallbackRef>& refs, sol::table keybinds, const string& identifier) {
			sol::table keybindTbl = keybinds[identifier];
			u32 def = keybindTbl.get<unsigned short>("default");
			
			auto onPress = keybindTbl.get<sol::optional<sol::function>>("on_press");
			auto onRelease = keybindTbl.get<sol::optional<sol::function>>("on_release");
			
			if (onPress) refs.emplace_back(input.events.bind(Input::CBType::KEY_PRESS, [=](u32 key, i32) {
				if (key == def) (*onPress)(); }));
			if (onRelease) refs.emplace_back(input.events.bind(Input::CBType::KEY_RELEASE, [=](u32 key, i32) {
				if (key == def) (*onRelease)(); }));
		}
	}
	
	
	/**
	 * Client method to register a keybind. Calls registerKeybind with the necessary parameters.
	 * There is no server register method (because that wouldn't make sense), but this method is here for consistency.
	 * Registers a keybind to the keybind handler.
	 *
	 * @param core - The core table to index for 'registered_keybinds', i.e. `_G['zepha']`.
	 * @param game - The input manager to add the keybind to.
	 * @param identifier - The identifier of the keybind to add.
	 */
	
	static void client(sol::table& core, Input& input, vec<CallbackRef>& refs, const string& identifier) {
		registerKeybind(input, refs, core.get<sol::table>("registered_keybinds"), identifier);
	}
};