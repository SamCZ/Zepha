//
// Created by aurailus on 2020-07-30.
//

#include <unordered_set>

#include "Dimension.h"

#include "../Lua.h"
#include "world/LocalWorld.h"
#include "world/ServerWorld.h"

void Api::Module::Dimension::bind() {
	core.set_function("create_dimension", Util::bind_this(this, &Dimension::createDimension));
	core.set_function("get_dimension", Util::bind_this(this, &Dimension::getDimension));
	core.set_function("get_default_dimension", Util::bind_this(this, &Dimension::getDefaultDimension));
	core.set_function("set_default_dimension", Util::bind_this(this, &Dimension::setDefaultDimension));
}

Api::Usertype::Dimension Api::Module::Dimension::createDimension(const std::string& identifier, sol::table data) {
	std::unordered_set<std::string> biomes{};
	auto biomesTbl = data.get<sol::optional<sol::table>>("biomes");
	
	if (biomesTbl) {
		for (auto& entry : *biomesTbl) {
			if (entry.second.is<std::string>()) biomes.insert(entry.second.as<std::string>());
		}
	}
	else biomes.insert("*");
	
	return Api::Usertype::Dimension(world.createDimension(identifier, biomes));
}

void Api::Module::Dimension::setDefaultDimension(const std::string& identifier) {
	world.setDefaultDimension(identifier);
}

sol::object Api::Module::Dimension::getDefaultDimension(const sol::this_state s) {
	auto dim = world.getDefaultDimension();
	if (!dim) return sol::nil;
	return sol::make_object(s, Api::Usertype::Dimension(dim));
}

sol::object Api::Module::Dimension::getDimension(sol::this_state s, const std::string& identifier) {
	auto dim = world.getDimension(identifier);
	if (!dim) return sol::nil;
	return sol::make_object(s, Api::Usertype::Dimension(dim));
}