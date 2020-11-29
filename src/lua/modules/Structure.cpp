#include "Structure.h"

#include "lua/Lua.h"
#include "game/Subgame.h"
#include "util/Schematic.h"
#include "game/def/BlockDef.h"
#include "game/atlas/DefinitionAtlas.h"

void Api::Module::Structure::bind() {
	core.set_function("create_structure", Util::bind_this(this, &Structure::create_structure));
}

sol::object Api::Module::Structure::create_structure(sol::table data) {
	auto origin = data.get<sol::optional<glm::vec3>>("origin");
	auto schematic = data.get<sol::table>("schematic");
	auto probability = data.get<float>("probability");
	
	unsigned int yWid = schematic.size();
	unsigned int zWid = schematic.get<sol::table>(1).size();
	unsigned int xWid = schematic.get<sol::table>(1).get<sol::table>(1).size();
	
	auto s = std::make_shared<::Schematic>();
	
	s->dimensions = { xWid, yWid, zWid };
	s->origin = origin ? glm::ivec3 { *origin } : glm::ivec3 {};
	s->blocks.reserve(xWid * yWid * zWid);
	s->probability = probability;
	
	for (unsigned int x = 1; x <= yWid; x++)
		for (unsigned int y = 1; y <= zWid; y++)
			for (unsigned int z = 1; z <= xWid; z++)
				s->blocks.push_back(game.getDefs().blockFromStr(schematic.
					get<sol::table>(y).get<sol::table>(z).get_or<std::string>(x, "")).index);
	
	return sol::make_object<std::shared_ptr<Schematic>>(lua, s);
}