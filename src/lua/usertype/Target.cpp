//
// Created by aurailus on 2020-07-27.
//

#include "Target.h"

#include "../Lua.h"

Api::Usertype::Target::Target(const ::Target& target) {
	if (target.type == ::Target::Type::BLOCK) {
		type = "block";
		dim = target.dim;
		pos = target.data.block.pos;
		pos_above = target.getAbovePos();
	}
	else if (target.type == ::Target::Type::ENTITY) {
		type = "entity";
		dim = target.dim;
		id = target.data.entity.id;
	}
	else type = "nothing";
}

void Api::Usertype::Target::bind(State, sol::state& lua, sol::table& core) {
	lua.new_usertype<Target>("Target",
		"type", sol::readonly(&Target::type),
		
		"id", sol::readonly(&Target::id),
		"dim", sol::readonly(&Target::dim),
		"pos", sol::readonly(&Target::pos),
		"pos_above", sol::readonly(&Target::pos_above)
	);
}