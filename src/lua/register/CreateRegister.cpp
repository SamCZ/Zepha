
#include "CreateRegister.h"

#include "lua/Lua.h"

namespace {
	
	/**
	 * Handles a register request from Lua. Accepts an identifier and a data table,
	 * adds it to the registered table, and calls the after function, if it exists.
	 *
	 * @param core - The core table, i.e. `_G['zepha']`.
	 * @param table - The register table, e.g `_G['registered_blocks']`.
	 * @param env - The sol::environment that the function is being called from.
	 * @param after - The function to execute after the element is added to the register table.
	 * @param identifier - The identifier of the element to add.
	 * @param data - The data table of the element to add.
	 */
	
	void registerFn(sol::table& core, const std::string& table, sol::environment env,
		std::function<void(std::string)> after, const std::string& identifier, const sol::table& data) {
		
		auto modName = env.get<std::string>("_MODNAME");
		std::string iden = identifier[0] == ':' ? modName + identifier : identifier;
		
		if (iden[0] == '!') iden = iden.substr(1, iden.length() - 1);
		else if (iden.length() <= modName.length() || iden.substr(0, modName.length() + 1) != modName + ":")
			throw std::runtime_error(identifier + " does not match calling mod name.");
		
		unsigned int splitters = std::count_if(iden.begin(), iden.end(), [](char c) { return c == ':'; });
		
		if (splitters > 2) throw std::runtime_error("Too many splitters in identifier " + identifier + ".");
		
		core[table][iden] = data;
		if (after) after(iden);
	}
}


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

void Api::Util::createRegister(sol::state& lua, sol::table& core,
	const std::string& name, std::function<void(std::string)> after, const std::string& table) {
	
	std::string tableName = "registered_" + (table.empty() ? name + "s" : table);
	
	core[tableName] = lua.create_table();
	
	core.set_function("register_" + name,
		[=, &core](sol::this_environment env, const std::string& identifier, sol::table data) {
			registerFn(core, tableName, static_cast<sol::environment>(env), after, identifier, data);
		});
}