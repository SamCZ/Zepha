#include "Structure.h"

#include "lua/Lua.h"
#include "game/Subgame.h"
#include "util/Structure.h"
#include "game/def/BlockDef.h"
#include "game/atlas/DefinitionAtlas.h"

void Api::Module::Structure::bind() {
	core.set_function("create_structure", Util::bind_this(this, &Structure::create_structure));
}

sol::object Api::Module::Structure::create_structure(sol::table data) {
	auto origin = data.get<sol::optional<glm::vec3>>("origin");
	auto probability = data.get<float>("probability");
	auto layout = data.get<sol::table>("layout");
	
	unsigned int yWid = layout.size();
	unsigned int zWid = layout.get<sol::table>(1).size();
	unsigned int xWid = layout.get<sol::table>(1).get<sol::table>(1).size();
	
	auto s = std::make_shared<::Structure>();
	
	s->dimensions = { xWid, yWid, zWid };
	s->origin = origin ? glm::ivec3 { *origin } : glm::ivec3 {};
	s->layout.reserve(xWid * yWid * zWid);
	s->probability = probability;
	
	for (unsigned int x = 1; x <= xWid; x++)
		for (unsigned int y = 1; y <= yWid; y++)
			for (unsigned int z = 1; z <= zWid; z++)
				s->layout.push_back(game.getDefs().blockFromStr(layout.
					get<sol::table>(y).get<sol::table>(z).get_or<std::string>(x, "")).index);
	
	return sol::make_object<std::shared_ptr<::Structure>>(lua, s);
}