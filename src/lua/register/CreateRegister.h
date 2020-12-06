#pragma once

#include <string>
#include <functional>
#include <sol/forward.hpp>

namespace Api::Util {
	
	/**
	 * Create a new register_* function in the core namespace and an associated registered_* table.
	 * The passed in function will be called after a new element has been inserted into the table using the
	 * generated function.
	 *
	 * @param lua - The Sol Lua State.
	 * @param core - The core table, i.e. `_G['zepha']`.
	 * @param name - The name of the element being registered. The name of the generated function will be 'register_[name]'.
	 * @param after - The function to run every time an element is registered.
	 * @param table - Optional, overrides the default table name and changes it to 'register_[table]'.
	 */
 
	void createRegister(sol::state& lua, sol::table& core, const std::string& name,
		std::function<void(std::string)> after = nullptr, const std::string& table = "");
}